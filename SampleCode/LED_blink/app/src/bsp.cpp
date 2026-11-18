/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG LED blink example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes target board peripherals:
 *          - Calls Led::initialize() to set PB14 as GPIO output and turn off the red LED.
 * @see Led::initialize, Led
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Initialize on-board user LED (PB14)
	// --------------------------------------------------
	Led::initialize();
}


