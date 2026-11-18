/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for on-board LED blinking using yss OS delay.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes the yss OS kernel and board hardware. In the main task loop,
 *          periodically toggles the on-board red LED with a 500ms delay between states
 *          using the thread::delay non-blocking RTOS delay service.
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

	// Main blinking loop
	while(1)
	{
		// Wait 500 ms and illuminate LED
		thread::delay(500);
		Led::red(true);

		// Wait 500 ms and turn off LED
		thread::delay(500);
		Led::red(false);
	}
}



