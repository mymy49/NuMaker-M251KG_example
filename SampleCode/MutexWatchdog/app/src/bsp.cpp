/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG Mutex Watchdog example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Configures GPIO port B pin 14 as output mode to control the on-board red LED.
 * @see setLed, gpioB
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// Configure PB14 as push-pull output for status indicator LED
	gpioB.setAsOutput(14);
}

/**
 * @details Drives PB14 LOW to turn ON the active-low LED, or HIGH to turn it OFF.
 * @see initializeBoard, gpioB
 * @todo none
 * @bug none
 */
void setLed(bool on)
{
	// Invert state for active-low circuit logic
	gpioB.setOutput(14, !on);
}

