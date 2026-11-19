/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG UART example.
 * @details Configures GPIO Port B pins 12 and 13 for UART0 communication (connected to the
 *          on-board Nu-Link-Me Virtual COM Port) and initializes the user indicator LED.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes UART0 peripheral and on-board status LED:
 *          1. Sets PB12 as `UART0_RXD` and PB13 as `UART0_TXD` alternate functions.
 *          2. Configures UART0 in normal 8N1 mode at 115200 baud with a 64-byte internal receive ring buffer.
 *          3. Enables the UART0 peripheral clock and NVIC receive/transmit interrupts.
 *          4. Initializes the on-board user LED via `Led::initialize()`.
 * @see uart0, Uart::config_t, Led::initialize, gpioB
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure GPIO pins for UART0 (PB12: RXD, PB13: TXD)
	// --------------------------------------------------
	// Multiplex PB12 to UART0 Receive Data input
	gpioB.setAsAltFunc(12, Gpio::PB12_UART0_RXD);
	// Multiplex PB13 to UART0 Transmit Data output
	gpioB.setAsAltFunc(13, Gpio::PB13_UART0_TXD);

	// --------------------------------------------------
	// 2. Configure UART0 Communication Parameters (115200 8N1)
	// --------------------------------------------------
	Uart::config_t uart0Config =
	{
		Uart::MODE_NORMAL,		// mode_t mode: Normal asynchronous operating mode
		115200,					// uint32_t baudrate: Baud rate setting (115200 bps)
		Uart::STOP_1BIT,		// stopbit_t stopbit: 1 stop bit
		Uart::PARITY_NONE,		// parityBit_t parity: No parity bit
		nullptr,				// void *rcvBuf: Allocate receive ring buffer dynamically on heap
		64,						// uint32_t rcvBufSize: Internal ring buffer size (64 bytes)
	};

	// Enable APB clock for UART0 peripheral
	uart0.enableClock();
	// Apply baud rate, frame format, and allocate ring buffer
	uart0.initialize(uart0Config);
	// Enable UART0 NVIC interrupt for interrupt-driven RX buffering
	uart0.enableInterrupt();

	// --------------------------------------------------
	// 3. Initialize user LED (PB14)
	// --------------------------------------------------
	// Configure PB14 as output and initialize LED to OFF
	Led::initialize();
}


