/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main entry point for the 2.8-inch TFT Touch Shield demonstration application.
 * @details Coordinates system initialization, non-volatile touch calibration loading from flash,
 *          and GUI page task scheduling via the yss OS `TaskManager`.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>
#include <page.h>
#include <util/runtime.h>
#include "memory.h"

/**
 * @details Initializes the yss OS kernel, target board hardware peripherals, and non-volatile
 *          flash memory. Registers the 4 GUI page tasks into the `page` TaskManager (logo, main menu,
 *          touch calibration, drawing app). First presents the startup logo splash screen for 5 seconds.
 *          If valid touch calibration data exists in flash (`memory.touchInitFlag == true`), advances
 *          directly to the main menu page; otherwise, launches the 2-point touch calibration page.
 *          The main thread then yields execution in an infinite loop.
 * @see initializeYss, initializeBoard, initializeMemory, page, TaskManager::add, TaskManager::play
 * @todo none
 * @bug none
 */
int main(void)
{
	PointerDevice::event_t event;

	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and peripherals
	// --------------------------------------------------
	initializeBoard();

	// --------------------------------------------------
	// 3. Initialize non-volatile memory and calibration
	// --------------------------------------------------
	initializeMemory();

	// --------------------------------------------------
	// 4. Register GUI pages into the task manager
	// --------------------------------------------------
	pageDisplayLogoId = page.add(pageDisplayLogo, 1024);
	pageDisplayMainId = page.add(pageDisplayMain, 2048);
	pageDisplayTouchCalibrationId = page.add(pageDisplayTouchCalibration, 2048);
	pageDisplayDrawingId = page.add(pageDisplayDrawing, 2048);

	// Launch startup logo splash screen task
	page.play(pageDisplayLogoId);

	// Display splash screen for 5000 ms (5 seconds)
	thread::delay(5000);
	
	// Transition to main menu if touch calibration is already stored in flash; otherwise start calibration
	if(memory.touchInitFlag)
		page.play(pageDisplayMainId);
	else
		page.play(pageDisplayTouchCalibrationId);

	// Main loop: yield CPU execution to active GUI page and touch dispatcher threads
	while(1)
	{
		thread::yield();
	}
}



