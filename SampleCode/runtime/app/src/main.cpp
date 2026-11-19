/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Demonstration of high-resolution runtime execution time tracking using yss OS.
 * @details Repeatedly queries the monotonic system uptime counter (`runtime::getMsec()`)
 *          backed by a hardware timer and outputs the elapsed milliseconds to the debug console.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>
#include <util/runtime.h>

/**
 * @details Initializes the yss OS kernel and board hardware. Continuously queries the
 *          elapsed system uptime in milliseconds via `runtime::getMsec()` and prints the
 *          32-bit value to the SWD debug console using a carriage return (`\r`) to update
 *          the readout in place.
 * @see initializeYss, initializeBoard, runtime::getMsec
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
	// 2. Initialize board hardware
	// --------------------------------------------------
	initializeBoard();

	// Runtime measurement display loop
	while(1)
	{
		// Query current elapsed uptime in milliseconds from the hardware runtime timer and print in place
		debug_printf("%d\r", (uint32_t)runtime::getMsec());
	}
}



