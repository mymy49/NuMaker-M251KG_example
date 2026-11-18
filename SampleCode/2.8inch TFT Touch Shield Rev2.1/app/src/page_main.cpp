/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_main.cpp
 * @brief Main application launcher page implementation.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/drawing.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <page.h>

int16_t pageDisplayMainId;

/**
 * @details Callback triggered when the drawing application icon is pressed.
 *          Transitions the active page to pageDisplayDrawingId.
 * @see page, pageDisplayDrawingId
 * @todo none
 * @bug none
 */
static void callback_drawing(PointerDevice::event_t event)
{
	switch(event.event)
	{
	case PointerDevice::EVENT_TOUCH_DOWN :
		// Switch to drawing application page
		page.play(pageDisplayDrawingId);
		break;

	case PointerDevice::EVENT_TOUCH_UP :
		break;
	
	default :	
		break;
	}
}

/**
 * @details Renders the main launcher dashboard:
 *          1. Fills the screen with black background.
 *          2. Draws the drawing application icon bitmap at (20, 20).
 *          3. Registers a touch boundary of {20, 20, 90, 90} with callback_drawing.
 *          4. Enters an infinite yield loop waiting for user touch interaction.
 * @see lcd, drawing, dispatcher, callback_drawing
 * @todo none
 * @bug none
 */
void pageDisplayMain(void*)
{
	lcd.lock();

	// Clear display with black background
	lcd.setBackgroundColor({0x00, 0x00, 0x00});
	lcd.clear();
	
	// Draw app icon bitmap
	lcd.drawBitmap({20, 20}, drawing);

	lcd.unlock();
		
	// Bind icon touch region to callback
	dispatcher.clear();
	dispatcher.add({20, 20, 90, 90}, callback_drawing);

	while(true)
	{
		thread::yield();
	}
}


