/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG LED blink example.
 * @details Implements board hardware setup by invoking the common `Led::initialize()` driver.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes target board peripherals:
 *          - Calls `Led::initialize()` to configure GPIO Port B Pin 14 (PB14) as push-pull output
 *            and drive it HIGH to turn off the active-low red LED.
 * @see Led::initialize, Led::red
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Initialize on-board user LED (PB14)
	// --------------------------------------------------
	// Configure PB14 as push-pull output and ensure the LED starts in the OFF state
	Led::initialize();
}


