/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file config.h
 * @brief Board and system configuration header for Nuvoton M251 series.
 * @details This file defines operating system parameters, clock sources, and peripheral enable flags.
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
/// Select TIMER peripheral to support runtime measurement (RUNTIME_TIMER0, 1, 2, 3)
#define YSS_RUNTIME_TIMER	RUNTIME_TIMER0

/// Select TIMER peripheral to support microsecond/millisecond delay operations
#define YSS_DELAY_TIMER		DELAY_TIMER1

/// Number of SysTick clock cycles allocated per thread timeslice
#define THREAD_GIVEN_CLOCK	20000

/// Maximum number of threads that can be registered in the scheduler
#define MAX_THREAD			16

/// Fill thread stack with 0xAA pattern for stack high-water mark monitoring (true, false)
#define FILL_THREAD_STACK	true

// ####################### KEY Configuration #######################
/// Maximum number of virtual keys supported by the OS (0 = feature disabled)
#define NUM_OF_YSS_KEY		0

// ###################### Peripheral Enables ######################
// Change false to true to enable the corresponding peripheral driver.

/// BPWM0 peripheral driver enable switch
#define BPWM0_ENABLE		false
/// BPWM1 peripheral driver enable switch
#define BPWM1_ENABLE		false

/// Hardware CRC peripheral driver enable switch
#define CRC_ENABLE			true

/// EADC peripheral driver enable switch
#define EADC_ENABLE			false

/// I2C0 peripheral driver enable switch
#define I2C0_ENABLE			false
/// I2C1 peripheral driver enable switch
#define I2C1_ENABLE			false

/// I2S0 peripheral driver enable switch
#define I2S0_ENABLE			false

/// PWM0 peripheral driver enable switch
#define PWM0_ENABLE			true
/// PWM1 peripheral driver enable switch
#define PWM1_ENABLE			false
/// PWM2 peripheral driver enable switch
#define PWM2_ENABLE			false
/// PWM3 peripheral driver enable switch
#define PWM3_ENABLE			false

/// QSPI0 peripheral driver enable switch
#define	QSPI0_ENABLE		false

/// SPI0 peripheral driver enable switch
#define SPI0_ENABLE			true

/// TIMER0 peripheral driver enable switch
#define TIMER0_ENABLE		false
/// TIMER1 peripheral driver enable switch
#define TIMER1_ENABLE		false
/// TIMER2 peripheral driver enable switch
#define TIMER2_ENABLE		false
/// TIMER3 peripheral driver enable switch
#define TIMER3_ENABLE		false

/// UART0 peripheral driver enable switch
#define UART0_ENABLE		false
/// UART1 peripheral driver enable switch
#define UART1_ENABLE		false
/// UART2 peripheral driver enable switch
#define UART2_ENABLE		false

/// USPI0 (USCI SPI) peripheral driver enable switch
#define USPI0_ENABLE		false
/// UUART0 (USCI UART) peripheral driver enable switch
#define UUART0_ENABLE		false

#endif


