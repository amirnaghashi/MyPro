/*
 * GpioPort.cpp
 *
 *  Created on: 5 Nov 2026
 *      Author: amirn
 */

#include "GpioPort.h"
#include "Defines.h"

#include <sys/mman.h>
#include <hw/inout.h>

namespace hal{

GpioPort::~GpioPort() {
    close();
}

bool GpioPort::open(uint64_t base) {
    if (base_ != 0) return false;

    const uintptr_t mapped = mmap_device_io(IO_SIZE_MMAP, base);
    if (mapped == MAP_DEVICE_FAILED) return false;

    base_ = mapped;
    return true;
}

void GpioPort::close() {
    if (base_ != 0) {
        munmap_device_io(base_, IO_SIZE_MMAP);
        base_ = 0;
    }
}

void GpioPort::set(uint32_t mask) {
    out32(base_ + GPIO_OFFSET_SETDATAOUT, mask);
}

void GpioPort::clear(uint32_t mask) {
    out32(base_ + GPIO_OFFSET_CLRDATAOUT, mask);
}

uint32_t GpioPort::read() const {
    return in32(base_ + GPIO_OFFSET_DATAIN);
}

void GpioPort::makeOutput(uint32_t mask) {
    out32(base_ + GPIO_OFFSET_OE, in32(base_ + GPIO_OFFSET_OE) & ~mask);
}

} // namespace hal
