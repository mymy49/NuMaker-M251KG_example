/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for dual-channel EADC acquisition.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes the yss OS and target board peripherals. Continually reads
 *          the filtered 14-bit digital conversion values from EADC channels 0 and 1,
 *          printing results to the debug console.
 * @see initializeYss, initializeBoard, eadc
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
		// Print current converted ADC values for Channel 0 (PB0) and Channel 1 (PB1)
		debug_printf("%d, %d\n", eadc.getResult(0), eadc.getResult(1));
	}
}



