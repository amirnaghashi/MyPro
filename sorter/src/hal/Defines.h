/*
 * Defines.h
 *
 *  Created on: 5 Nov 2026
 *      Author: Amirhossein Naghashi
 */

#ifndef DEFINES_H_
#define DEFINES_H_

// GPIO base addresses and register offsets, spruh73l.pdf
#define IO_SIZE_MMAP        0x1000
#define GPIO_PORT0          0x44E07000
#define GPIO_PORT1          0x4804C000
#define GPIO_PORT2          0x481AC000

#define GPIO_OFFSET_OE       0x134
#define GPIO_OFFSET_DATAIN   0x138
#define GPIO_OFFSET_DATAOUT  0x13C
#define GPIO_OFFSET_CLRDATAOUT  0x190
#define GPIO_OFFSET_SETDATAOUT  0x194

/* Pin map based on ESEP task description */

// GPIO0 - inputs                                          active level
#define GPIO0_LB_START       2   // Workpiece at inlet        low
#define GPIO0_LB_SORTER      5   // Workpiece at sorter       low
#define GPIO0_METAL          7   // Workpiece is metal        high
#define GPIO0_SORTER_STATE  14   // Switch open               high (ejector: idle = 1)
#define GPIO0_LB_RAMP       15   // Ramp full                 low
#define GPIO0_LB_END        20   // Workpiece at outlet       low
#define GPIO0_BTN_START     22   // Start button              high
#define GPIO0_BTN_STOP      23   // Stop button               low
#define GPIO0_BTN_RESET     26   // Reset button              high
#define GPIO0_ESTOP         27   // E-Stop                    low

// GPIO1 - outputs
#define GPIO1_MOTOR_RIGHT   12
#define GPIO1_MOTOR_LEFT    13
#define GPIO1_MOTOR_SLOW    14
#define GPIO1_MOTOR_STOP    15
#define GPIO1_SL_RED        16
#define GPIO1_SL_YELLOW     17
#define GPIO1_SL_GREEN      18
#define GPIO1_SORTER        19   // Open switch / extend ejector

// GPIO2 - outputs
#define GPIO2_LED_START      2
#define GPIO2_LED_RESET      3
#define GPIO2_LED_Q1         4
#define GPIO2_LED_Q2         5


// Bit mask for pin n (0..31) of a GPIO bank
#define GPIO_BIT(n) (1u << (n))



#endif /* DEFINES_H_ */
