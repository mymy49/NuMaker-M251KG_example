/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_main.cpp
 * @brief Main application launcher page implementation.
 * @details Renders the main dashboard screen with the Drawing application launcher icon
 *          and registers its touch hit-box with the global `TouchEventDispatcher`.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/drawing.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <page.h>

/**
 * @brief Registered task identifier for the main menu launcher page.
 */
int16_t pageDisplayMainId;

/**
 * @details Callback triggered when the drawing application icon (region `{20, 20, 90, 90}`) is pressed.
 *          On `PointerDevice::EVENT_TOUCH_DOWN`, transitions the active page in `page` TaskManager
 *          to `pageDisplayDrawingId`.
 * @see page, pageDisplayDrawingId, TaskManager::play
 * @todo none
 * @bug none
 */
void callback_drawing(PointerDevice::event_t event)
{
	// Inspect the type of touch event reported by the dispatcher
	switch(event.event)
	{
	case PointerDevice::EVENT_TOUCH_DOWN :
		// Switch active GUI task to the free-hand drawing application page
		page.play(pageDisplayDrawingId);
		break;

	case PointerDevice::EVENT_TOUCH_UP :
		// No action needed on touch release
		break;
	
	default :	
		break;
	}
}

/**
 * @details Renders the main launcher dashboard:
 *          1. Acquires the LCD mutex lock and fills the 240x320 screen with a solid black background.
 *          2. Draws the 90x90 drawing application icon bitmap (`drawing`) at coordinate (20, 20).
 *          3. Clears any previous touch regions and registers the `{20, 20, 90, 90}` bounding box
 *             with `callback_drawing` in `dispatcher`.
 *          4. Enters an infinite yield loop waiting for user touch interaction.
 * @see lcd, drawing, dispatcher, callback_drawing
 * @todo none
 * @bug none
 */
void pageDisplayMain(void*)
{
	// Acquire exclusive LCD access before drawing the main menu
	lcd.lock();

	// Clear the entire display with a solid black background
	lcd.setBackgroundColor({0x00, 0x00, 0x00});
	lcd.clear();
	
	// Draw the 90x90 drawing application launcher icon at (20, 20)
	lcd.drawBitmap({20, 20}, drawing);

	// Release LCD mutex lock
	lcd.unlock();
		
	// Remove previous page touch listeners and bind the 90x90 icon region to callback_drawing
	dispatcher.clear();
	dispatcher.add({20, 20, 90, 90}, callback_drawing);

	// Idle loop: yield processor time while waiting for asynchronous touch callbacks
	while(true)
	{
		thread::yield();
	}
}


