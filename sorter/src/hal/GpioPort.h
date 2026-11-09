/*
 * GpioPort.h
 *
 *  Created on: 5 Nov 2026
 *      Author: amirn
 */

#ifndef HAL_GPIOPORT_H_
#define HAL_GPIOPORT_H_

#include <cstdint>

namespace hal {

/**
 * @brief Access to one GPIO bank of the AM335x.
 *
 * Maps the register block of one bank and provides atomic set/clear access
 * to the outputs and read access to the inputs. All register offsets are
 * encapsulated here. Pins are addressed by bit mask (see GPIO_BIT).
 */
class GpioPort {
public:
    GpioPort() = default;
    ~GpioPort();
    GpioPort(const GpioPort&) = delete;
    GpioPort& operator=(const GpioPort&) = delete;

    /**
     * @brief Maps the register block of a GPIO bank.
     * @param base Physical base address of the bank (e.g. GPIO_PORT1).
     * @return true on success; false if mapping failed or the port is already open.
     * @pre   The calling process has I/O privileges (ThreadCtl(_NTO_TCTL_IO)).
     */
    bool open(uint64_t base);

    /**
     * @brief Unmaps the register block. Safe to call if not open.
     */
    void close();

    /**
     * @brief Sets all pins in mask to high (atomic, other pins unaffected).
     * @pre   open() returned true.
     */
    void set(uint32_t mask);

    /**
     * @brief Sets all pins in mask to low (atomic, other pins unaffected).
     * @pre   open() returned true.
     */
    void clear(uint32_t mask);

    /**
     * @brief Reads the input level of all 32 pins (DATAIN).
     * @pre   open() returned true.
     */
    uint32_t read() const;

    /**
     * @brief Configures the pins in mask as outputs; other pins unchanged.
     * @pre   open() returned true.
     * @note  Read-modify-write on OE, not atomic. Call only during
     *        single-threaded initialisation.
     */
    void makeOutput(uint32_t mask);

private:
    uintptr_t base_ = 0;  // mapped address, 0 = not open
};

} // namespace hal

#endif /* HAL_GPIOPORT_H_ */
