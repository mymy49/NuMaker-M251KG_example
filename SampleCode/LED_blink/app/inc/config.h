/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file config.h
 * @brief Board and system configuration header for NuMaker-M251KG LED blink example.
 * @details Defines system clock parameters, scheduler limits, and peripheral enable switches.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef YSS_CONFIG__H_
#define YSS_CONFIG__H_

// ####################### External Crystal Clock Frequency #######################
// If HSE_CLOCK_FREQ is defined below, PLL configuration uses HSE as clock source.
// If not defined, PLL configuration uses HSI clock source instead.
// The crystal frequency must be an integer multiple of 4 MHz.
//#define HSE_CLOCK_FREQ 12000000

// ####################### Scheduler Configuration #######################
// Select TIMER peripheral to support runtime measurement (RUNTIME_TIMER0, 1, 2, 3)
#define YSS_TIMER			RUNTIME_TIMER0

// Number of SysTick clock cycles allocated per thread timeslice
#define THREAD_GIVEN_CLOCK	20000

// Maximum number of threads that can be registered
#define MAX_THREAD			16

// Fill thread stack with 0xAA pattern for stack monitoring (true, false)
#define FILL_THREAD_STACK	false

// ####################### GUI Configuration #######################
// GUI library Enable (true, false)
#define USE_GUI				false

// ####################### KEY Configuration #######################
// Maximum number of virtual keys (0 = feature disabled)
#define NUM_OF_YSS_KEY		0

// ###################### Peripheral Enables ######################
// Change false to true to enable the corresponding peripheral driver.

// BPWM Enable
#define BPWM0_ENABLE		false
#define BPWM1_ENABLE		false

// CRC Enable
#define CRC_ENABLE			true

// EADC Enable
#define EADC_ENABLE			false

// I2C Enable
#define I2C0_ENABLE			false
#define I2C1_ENABLE			false

// I2S Enable
#define I2S0_ENABLE			false

// PWM Enable
#define PWM0_ENABLE			false
#define PWM1_ENABLE			false
#define PWM2_ENABLE			false
#define PWM3_ENABLE			false

#define	QSPI0_ENABLE		false

// SPI Enable
#define SPI0_ENABLE			false

// TIMER Enable
#define TIMER0_ENABLE		false
#define TIMER1_ENABLE		false
#define TIMER2_ENABLE		false
#define TIMER3_ENABLE		false

// UART Enable
#define UART0_ENABLE		false
#define UART1_ENABLE		false
#define UART2_ENABLE		false

// USCI Enable
#define USPI0_ENABLE		false
#define UUART0_ENABLE		false

#endif


