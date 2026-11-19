/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for on-board LED blinking using yss OS delay.
 * @details Toggles the NuMaker-M251KG red user LED every 500 milliseconds using the
 *          non-blocking `thread::delay()` scheduler service.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes the yss OS kernel and board hardware. In the main task loop,
 *          periodically toggles the on-board red LED with a 500 ms interval between ON and OFF states
 *          using the `thread::delay()` non-blocking RTOS delay service (yielding CPU cycles during the wait).
 * @see initializeYss, initializeBoard, Led::red, thread::delay
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
	// 2. Initialize board hardware and on-board LED
	// --------------------------------------------------
	initializeBoard();

	// Main blinking loop (1 Hz cycle: 500 ms ON, 500 ms OFF)
	while(1)
	{
		// Suspend thread for 500 ms and then turn ON the red LED (PB14 driven LOW)
		thread::delay(500);
		Led::red(true);

		// Suspend thread for 500 ms and then turn OFF the red LED (PB14 driven HIGH)
		thread::delay(500);
		Led::red(false);
	}
}



