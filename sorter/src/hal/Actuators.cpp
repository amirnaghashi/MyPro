/*
 * Actuators.cpp
 *
 *  Created on: 6 Nov 2026
 *      Author: amirn
 */

#include "Actuators.h"
#include "Defines.h"

#include <cassert>
#include <cerrno>
#include <sched.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>

namespace hal {

namespace {

/* Timing constants. Values marked TODO are placeholders until measured. */
constexpr int T_DETECT_SETTLE_MS = 250;   // HW_REQ_7: longest measured return time (176 ms) + margin
constexpr int T_SWITCH_OPEN_MS   = 1000;  // TODO: measure with workpiece (time until it has passed)
constexpr int T_EJECT_PULSE_MS   = 300;   // TODO: measure with workpiece (until it is in the ramp)
constexpr int T_BLINK_TICK_MS    = 250;   // BLINKFAST toggles every tick (2 Hz), BLINKSLOW every 2nd tick (1 Hz)

/* Priorities (QNX: 1 = lowest; default application priority is 10).
 * TODO: agree on the system-wide priority plan with the team. */
constexpr int PRIO_WORKER       = 20;  // base priority of the worker (floats with received pulses)
constexpr int PRIO_SORTER_TIMER = 30;  // closing the switch is a safety action: above all application threads

/* Pulse codes for the worker channel. */
enum : int {
    CODE_BELT_RUN = _PULSE_CODE_MINAVAIL,
    CODE_BELT_STOP,
    CODE_LET_PASS,
    CODE_SORT_OUT,
    CODE_STACKLIGHT,
    CODE_LED,
    CODE_SORTER_TIMER,
    CODE_BLINK_TIMER,
    CODE_SHUTDOWN
};

constexpr uint32_t GPIO1_USED =
    GPIO_BIT(GPIO1_MOTOR_RIGHT) | GPIO_BIT(GPIO1_MOTOR_LEFT) |
    GPIO_BIT(GPIO1_MOTOR_SLOW)  | GPIO_BIT(GPIO1_MOTOR_STOP) |
    GPIO_BIT(GPIO1_SL_RED)      | GPIO_BIT(GPIO1_SL_YELLOW)  |
    GPIO_BIT(GPIO1_SL_GREEN)    | GPIO_BIT(GPIO1_SORTER);

constexpr uint32_t GPIO2_USED =
    GPIO_BIT(GPIO2_LED_START) | GPIO_BIT(GPIO2_LED_RESET) |
    GPIO_BIT(GPIO2_LED_Q1)    | GPIO_BIT(GPIO2_LED_Q2);

/* Order must match enum Color { RED, GREEN, YELLOW }. */
constexpr uint32_t LIGHT_MASK[3] = {
    GPIO_BIT(GPIO1_SL_RED), GPIO_BIT(GPIO1_SL_GREEN), GPIO_BIT(GPIO1_SL_YELLOW)
};

/* Order must match enum Led { Q1, Q2, LED_BTN_START, LED_BTN_RESET }. */
constexpr uint32_t LED_MASK[4] = {
    GPIO_BIT(GPIO2_LED_Q1), GPIO_BIT(GPIO2_LED_Q2),
    GPIO_BIT(GPIO2_LED_START), GPIO_BIT(GPIO2_LED_RESET)
};

/* Pulse value for stackLight()/led(): index in bits 8..15, mode in bits 0..7. */
int encode(int index, LightMode mode) {
    return (index << 8) | static_cast<int>(mode);
}

/** Arms a timer once (periodic = false) or periodically. Returns false on failure. */
bool armMs(timer_t timer, int ms, bool periodic) {
    itimerspec its{};
    its.it_value.tv_sec  = ms / 1000;
    its.it_value.tv_nsec = static_cast<long>(ms % 1000) * 1000000L;
    if (periodic) its.it_interval = its.it_value;
    return timer_settime(timer, 0, &its, nullptr) == 0;
}

} // namespace

/* ======================= lifecycle (caller thread) ======================= */

Actuators::~Actuators() {
    shutdown();
}

bool Actuators::init() {
    if (initialised_) return false;

    // Must happen before the worker is started: threads inherit I/O privileges.
    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) return false;

    if (!gpio0_.open(GPIO_PORT0) || !gpio1_.open(GPIO_PORT1) || !gpio2_.open(GPIO_PORT2)) {
        shutdown();
        return false;
    }
    portsOpen_ = true;

    gpio1_.makeOutput(GPIO1_USED);
    gpio2_.makeOutput(GPIO2_USED);   // GPIO2 is configured as input after boot
    allOff();
    sorter_ = detectSorter();

    chid_ = ChannelCreate(0);
    if (chid_ == -1) { shutdown(); return false; }

    coid_ = ConnectAttach(0, 0, chid_, _NTO_SIDE_CHANNEL, 0);
    if (coid_ == -1) { shutdown(); return false; }

    if (!createTimer(sorterTimer_, sorterTimerValid_, CODE_SORTER_TIMER, PRIO_SORTER_TIMER) ||
        !createTimer(blinkTimer_,  blinkTimerValid_,  CODE_BLINK_TIMER,  SIGEV_PULSE_PRIO_INHERIT)) {
        shutdown();
        return false;
    }

    if (!startWorker()) { shutdown(); return false; }

    if (!armMs(blinkTimer_, T_BLINK_TICK_MS, true)) { shutdown(); return false; }

    initialised_ = true;
    return true;
}

bool Actuators::startWorker() {
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0) return false;

    sched_param param{};
    param.sched_priority = PRIO_WORKER;

    bool ok = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) == 0 &&
              pthread_attr_setschedpolicy(&attr, SCHED_FIFO) == 0 &&
              pthread_attr_setschedparam(&attr, &param) == 0;

    // pthread_create returns an error number (not -1/errno).
    ok = ok && pthread_create(&worker_, &attr, &Actuators::workerEntry, this) == 0;

    pthread_attr_destroy(&attr);
    workerRunning_ = ok;
    return ok;
}

void* Actuators::workerEntry(void* self) {
    static_cast<Actuators*>(self)->workerLoop();
    return nullptr;
}

void Actuators::shutdown() {
    if (workerRunning_) {
        MsgSendPulse(coid_, -1, CODE_SHUTDOWN, 0);
        pthread_join(worker_, nullptr);
        workerRunning_ = false;
    }
    // Worker is stopped from here on: this thread is the only writer again.
    if (blinkTimerValid_)  { timer_delete(blinkTimer_);  blinkTimerValid_  = false; }
    if (sorterTimerValid_) { timer_delete(sorterTimer_); sorterTimerValid_ = false; }
    if (coid_ != -1) { ConnectDetach(coid_);  coid_ = -1; }
    if (chid_ != -1) { ChannelDestroy(chid_); chid_ = -1; }

    if (portsOpen_) {
        allOff();
        portsOpen_ = false;
    }
    gpio0_.close();
    gpio1_.close();
    gpio2_.close();
    initialised_ = false;
}

bool Actuators::createTimer(timer_t& timer, bool& valid, int code, int priority) {
    sigevent ev;
    SIGEV_PULSE_INIT(&ev, coid_, priority, code, 0);
    if (timer_create(CLOCK_MONOTONIC, &ev, &timer) == -1) return false;
    valid = true;
    return true;
}

Actuators::SorterType Actuators::detectSorter() {
    gpio1_.clear(GPIO_BIT(GPIO1_SORTER));
    usleep(T_DETECT_SETTLE_MS * 1000);
    // HW_REQ_7: idle level 0 -> switch, 1 -> ejector
    return (gpio0_.read() & GPIO_BIT(GPIO0_SORTER_STATE)) ? SorterType::EJECTOR
                                                          : SorterType::SWITCH;
}

void Actuators::allOff() {
    gpio1_.clear(GPIO1_USED);
    gpio2_.clear(GPIO2_USED);
    gpio1_.set(GPIO_BIT(GPIO1_MOTOR_STOP));
}

/* ======================= public interface (caller thread) ======================= */

void Actuators::send(int code, int value) {
    assert(initialised_);
    MsgSendPulse(coid_, -1, code, value);
}

void Actuators::beltRun(BeltSpeed speed) { send(CODE_BELT_RUN, static_cast<int>(speed)); }
void Actuators::beltStop()               { send(CODE_BELT_STOP, 0); }
void Actuators::letPass()                { send(CODE_LET_PASS, 0); }
void Actuators::sortOut()                { send(CODE_SORT_OUT, 0); }

void Actuators::stackLight(Color color, LightMode lightmode) {
    send(CODE_STACKLIGHT, encode(static_cast<int>(color), lightmode));
}

void Actuators::led(Led led, LightMode lightmode) {
    send(CODE_LED, encode(static_cast<int>(led), lightmode));
}

/* ======================= worker thread ======================= */

void Actuators::workerLoop() {
    for (;;) {
        _pulse pulse;
        if (MsgReceivePulse(chid_, &pulse, sizeof(pulse), nullptr) == -1) {
            if (errno == EINTR) continue;
            return;  // channel gone: stop instead of spinning
        }

        const int value = pulse.value.sival_int;
        const int index = value >> 8;
        const auto mode = static_cast<LightMode>(value & 0xFF);

        switch (pulse.code) {
        case CODE_BELT_RUN:
            doBeltRun(static_cast<BeltSpeed>(value));
            break;
        case CODE_BELT_STOP:
            doBeltStop();
            break;
        case CODE_LET_PASS:
            if (sorter_ == SorterType::SWITCH) activateSorter(T_SWITCH_OPEN_MS);
            break;
        case CODE_SORT_OUT:
            if (sorter_ == SorterType::EJECTOR) activateSorter(T_EJECT_PULSE_MS);
            break;
        case CODE_SORTER_TIMER:
            gpio1_.clear(GPIO_BIT(GPIO1_SORTER));
            sorterActive_ = false;
            break;
        case CODE_STACKLIGHT:
            if (index >= 0 && index < 3) { lightMode_[index] = mode; applyLights(); }
            break;
        case CODE_LED:
            if (index >= 0 && index < 4) { ledMode_[index] = mode; applyLights(); }
            break;
        case CODE_BLINK_TIMER:
            ++blinkTick_;
            applyLights();
            break;
        case CODE_SHUTDOWN:
            return;
        default:
            break;
        }
    }
}

void Actuators::doBeltRun(BeltSpeed speed) {
    // Speed first, STOP released last: no transient fast run when starting slow.
    if (speed == BeltSpeed::SLOW) gpio1_.set(GPIO_BIT(GPIO1_MOTOR_SLOW));
    else                          gpio1_.clear(GPIO_BIT(GPIO1_MOTOR_SLOW));
    gpio1_.set(GPIO_BIT(GPIO1_MOTOR_RIGHT));
    gpio1_.clear(GPIO_BIT(GPIO1_MOTOR_STOP) | GPIO_BIT(GPIO1_MOTOR_LEFT));
}

void Actuators::doBeltStop() {
    // STOP first, then clear direction and speed.
    gpio1_.set(GPIO_BIT(GPIO1_MOTOR_STOP));
    gpio1_.clear(GPIO_BIT(GPIO1_MOTOR_RIGHT) | GPIO_BIT(GPIO1_MOTOR_LEFT) |
                 GPIO_BIT(GPIO1_MOTOR_SLOW));
}

void Actuators::activateSorter(int durationMs) {
    if (sorterActive_) return;  // D4: ignore while active -> energising time stays bounded
    sorterActive_ = true;
    gpio1_.set(GPIO_BIT(GPIO1_SORTER));
    if (!armMs(sorterTimer_, durationMs, false)) {
        // Fail-safe: never leave the sorter energised without a running timer.
        gpio1_.clear(GPIO_BIT(GPIO1_SORTER));
        sorterActive_ = false;
    }
}

bool Actuators::isOn(LightMode mode) const {
    switch (mode) {
    case LightMode::ON:        return true;
    case LightMode::BLINKFAST: return (blinkTick_ & 1u) == 0;
    case LightMode::BLINKSLOW: return (blinkTick_ & 2u) == 0;
    case LightMode::OFF:
    default:                   return false;
    }
}

void Actuators::applyLights() {
    uint32_t on1 = 0, off1 = 0;
    for (int i = 0; i < 3; ++i) (isOn(lightMode_[i]) ? on1 : off1) |= LIGHT_MASK[i];

    uint32_t on2 = 0, off2 = 0;
    for (int i = 0; i < 4; ++i) (isOn(ledMode_[i]) ? on2 : off2) |= LED_MASK[i];

    gpio1_.set(on1);
    gpio1_.clear(off1);
    gpio2_.set(on2);
    gpio2_.clear(off2);
}

} // namespace hal
