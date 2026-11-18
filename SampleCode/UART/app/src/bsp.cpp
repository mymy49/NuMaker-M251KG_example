/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG UART example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes UART0 peripheral and on-board status LED:
 *          1. Sets PB12 as UART0_RXD and PB13 as UART0_TXD alternate functions.
 *          2. Configures UART0 in normal 8N1 mode at 115200 baud with a 64-byte receive buffer.
 *          3. Enables UART0 peripheral clock and interrupts.
 *          4. Initializes user LED via Led::initialize().
 * @see uart0, Led::initialize, gpioB
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure GPIO pins for UART0 (PB12: RXD, PB13: TXD)
	// --------------------------------------------------
	gpioB.setAsAltFunc(12, Gpio::PB12_UART0_RXD);
	gpioB.setAsAltFunc(13, Gpio::PB13_UART0_TXD);

	// --------------------------------------------------
	// 2. Configure UART0 Communication Parameters (115200 8N1)
	// --------------------------------------------------
	Uart::config_t uart0Config =
	{
		Uart::MODE_NORMAL,		// mode_t mode: Normal asynchronous operating mode
		115200,					// uint32_t baudrate: Baud rate setting
		Uart::STOP_1BIT,		// stopbit_t stopbit: 1 stop bit
		Uart::PARITY_NONE,		// parityBit_t parity: No parity bit
		nullptr,				// void *rcvBuf: Optional custom receive buffer
		64,						// uint32_t rcvBufSize: Internal ring buffer size (64 bytes)
	};

	uart0.enableClock();
	uart0.initialize(uart0Config);
	uart0.enableInterrupt();

	// --------------------------------------------------
	// 3. Initialize user LED (PB14)
	// --------------------------------------------------
	Led::initialize();
}


