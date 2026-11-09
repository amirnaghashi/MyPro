// main.cpp – Integrationstest Actuators an der Anlage
#include "hal/Actuators.h"
#include <cstdio>
#include <unistd.h>

using namespace hal;

static void step(const char* text, unsigned seconds) {
    std::printf("%s\n", text);
    sleep(seconds);
}

int main() {
	Actuators impl;
	IActuators& act = impl;
    if (!act.init()) { std::printf("init() fehlgeschlagen\n"); return 1; }
    std::printf("init() ok\n");

    act.stackLight(Color::GREEN, LightMode::ON);      step("Ampel gruen an", 2);
    act.stackLight(Color::YELLOW, LightMode::BLINKSLOW); step("gelb blinkt langsam", 3);
    act.stackLight(Color::RED, LightMode::BLINKFAST);    step("rot blinkt schnell", 3);
    act.stackLight(Color::GREEN, LightMode::OFF);
    act.stackLight(Color::YELLOW, LightMode::OFF);
    act.stackLight(Color::RED, LightMode::OFF);

    act.led(Led::LED_BTN_START, LightMode::ON);  act.led(Led::LED_BTN_RESET, LightMode::ON);
    act.led(Led::Q1, LightMode::ON);             act.led(Led::Q2, LightMode::BLINKSLOW);
    step("LEDs Start/Reset/Q1 an, Q2 blinkt", 3);

    act.beltRun(BeltSpeed::SLOW); step("Band langsam", 2);
    act.beltRun(BeltSpeed::FAST); step("Band schnell", 2);
    act.beltStop();               step("Band stop", 1);

    act.letPass(); step("letPass(): nur Weiche oeffnet und schliesst selbst", 2);
    act.sortOut(); step("sortOut(): nur Auswerfer pulst", 2);

    std::printf("End, Destruktor schaltet alles ab\n");
    return 0;
}
