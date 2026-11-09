/*
 * IActuators.h
 *
 *  Created on: 5 Nov 2026
 *      Author: amirn
 */

#ifndef HAL_IACTUATORS_H_
#define HAL_IACTUATORS_H_

namespace hal {

enum class Color     { RED, GREEN, YELLOW };
enum class Led       { Q1, Q2, LED_BTN_START, LED_BTN_RESET };
enum class BeltSpeed { SLOW, FAST };
enum class LightMode { OFF, ON, BLINKSLOW, BLINKFAST };

/**
 * @brief Abstract interface for all actuators of one conveyor belt module.
 *
 * Hides the hardware details (GPIO registers, pin polarity, sorter variant)
 * from the control logic. All methods except init() require a successful
 * init() beforehand.
 */
class IActuators {
public:

    /**
     * @brief Initialises the actuator hardware and detects the sorter variant.
     *
     * The sorter variant (switch or ejector) is detected via GPIO0.14 while the
     * sorter output is inactive (HW_REQ_7).
     *
     * @return true if hardware access and variant detection succeeded, false otherwise.
     * @post   On success, all actuators are in their idle state.
     * @note   Calling any other method before a successful init() is undefined.
     */
    virtual bool init() = 0;

    /**
     * @brief Runs the belt in forward direction (right).
     *
     * Reverse direction is not supported. The implementation keeps
     * GPIO1_MOTOR_LEFT inactive at all times.
     *
     * @param speed Belt speed (SLOW or FAST).
     * @pre   init() returned true.
     */
    virtual void beltRun(BeltSpeed speed) = 0;

    /**
     * @brief Stops the belt.
     * @pre   init() returned true.
     */
    virtual void beltStop() = 0;

    /**
     * @brief Lets the current workpiece pass the sorter.
     *
     * Variant-independent; the implementation hides the actuator type.
     * - Switch:  opens and closes automatically after a fixed time
     *            (limited open time, coil must not overheat).
     * - Ejector: no action (passes when idle).
     *
     * @pre init() returned true.
     */
    virtual void letPass() = 0;

    /**
     * @brief Sorts the current workpiece out into the ramp.
     *
     * Variant-independent; the implementation hides the actuator type.
     * - Switch:  no action (closed when idle, workpiece is diverted).
     * - Ejector: short pulse, retracts automatically.
     *
     * @pre init() returned true.
     */
    virtual void sortOut() = 0;

    /**
     * @brief Sets the mode of one stack light colour.
     *
     * The last call per colour wins; colours are independent of each other.
     *
     * @param color     Colour to set.
     * @param lightmode OFF, ON, BLINKSLOW or BLINKFAST.
     * @pre   init() returned true.
     */
    virtual void stackLight(Color color, LightMode lightmode) = 0;

    /**
     * @brief Sets the mode of one LED.
     *
     * LEDs are independent of each other.
     *
     * @param led       LED to set.
     * @param lightmode OFF, ON, BLINKSLOW or BLINKFAST.
     * @pre   init() returned true.
     */
    virtual void led(Led led, LightMode lightmode) = 0;
    virtual ~IActuators() = default;

};
}

#endif /* HAL_IACTUATORS_H_ */
