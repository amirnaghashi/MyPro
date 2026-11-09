
/*
 * Actuators.h
 *
 *  Created on: 6 Nov 2026
 *      Author: amirn
 */

#ifndef HAL_ACTUATORS_H_
#define HAL_ACTUATORS_H_

#include "IActuators.h"
#include "GpioPort.h"

#include <cstdint>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>

namespace hal {

/**
 * @brief Actuator implementation for one conveyor belt module (BeagleBone Black, AM335x).
 *
 * Owns one GpioPort per GPIO bank (GPIO0 read-only for the sorter variant
 * detection, HW_REQ_7). After init(), all register writes are done by one
 * internal worker thread (POSIX thread, SCHED_FIFO): the public methods only
 * send QNX pulses to it.
 *
 * @note Thread-safe: concurrent calls do not corrupt internal state.
 *       Architectural rule: in normal operation only the control (FSM) thread
 *       issues commands; other components send events to the FSM instead.
 * @note Public methods return immediately; the effect on the outputs happens
 *       asynchronously in the worker thread (typically within microseconds).
 */
class Actuators : public IActuators {
public:
    Actuators() = default;

    /**
     * @brief Stops the worker, switches all outputs off and releases all resources.
     * @note  Prevents the switch from staying energised after a normal program end.
     *        Does not help on a crash or kill -9.
     */
    ~Actuators() override;

    /**
     * @brief Acquires I/O privileges, maps the GPIO banks, configures the outputs,
     *        detects the sorter variant and starts the worker thread.
     * @return false on any failure (all acquired resources are released again)
     *         or if already initialised.
     */
    bool init() override;

    void beltRun(BeltSpeed speed) override;
    void beltStop() override;

    void letPass() override;
    void sortOut() override;

    void stackLight(Color color, LightMode lightmode) override;
    void led(Led led, LightMode lightmode) override;

private:
    enum class SorterType { SWITCH, EJECTOR };

    // --- used by init() / destructor (caller thread, worker not running) ---
    SorterType detectSorter();
    void allOff();
    void shutdown();
    bool createTimer(timer_t& timer, bool& valid, int code, int priority);
    bool startWorker();

    // --- used by public methods (caller thread) ---
    void send(int code, int value);

    // --- used only by the worker thread ---
    static void* workerEntry(void* self);
    void workerLoop();
    void doBeltRun(BeltSpeed speed);
    void doBeltStop();
    void activateSorter(int durationMs);
    void applyLights();
    bool isOn(LightMode mode) const;

    GpioPort gpio0_;   // inputs, read-only
    GpioPort gpio1_;   // motor, stack light, sorter
    GpioPort gpio2_;   // button LEDs, Q1, Q2
    bool portsOpen_ = false;

    SorterType sorter_ = SorterType::SWITCH;  // valid after successful init()
    bool initialised_  = false;

    int       chid_ = -1;
    int       coid_ = -1;
    timer_t   sorterTimer_{};
    bool      sorterTimerValid_ = false;
    timer_t   blinkTimer_{};
    bool      blinkTimerValid_ = false;
    pthread_t worker_{};
    bool      workerRunning_ = false;

    // Worker-owned state: only accessed by the worker thread after init().
    bool     sorterActive_ = false;
    unsigned blinkTick_    = 0;
    LightMode lightMode_[3] = { LightMode::OFF, LightMode::OFF, LightMode::OFF };                  // index = Color
    LightMode ledMode_[4]   = { LightMode::OFF, LightMode::OFF, LightMode::OFF, LightMode::OFF };  // index = Led
};

} // namespace hal

#endif /* HAL_ACTUATORS_H_ */


