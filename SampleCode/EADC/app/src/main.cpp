/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for dual-channel EADC acquisition.
 * @details Continuously reads low-pass filtered 14-bit ADC results from EADC Channel 0 (PB0)
 *          and Channel 1 (PB1) and outputs them to the debug console.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes the yss OS kernel and target board peripherals. In an infinite polling
 *          loop, fetches the latest filtered 14-bit digital conversion values from EADC channels
 *          0 and 1 via `eadc.getResult()` and prints the pair of values to the SWD debug terminal.
 * @see initializeYss, initializeBoard, eadc, Adc::getResult
 * @todo none
 * @bug none
 */
int main(void)
{
	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and EADC channels
	// --------------------------------------------------
	initializeBoard();

	// Continuous conversion read loop
	while(1)
	{
		// Read and print current 14-bit filtered ADC values for Channel 0 (PB0) and Channel 1 (PB1)
		debug_printf("%d, %d\n", eadc.getResult(0), eadc.getResult(1));
	}
}



