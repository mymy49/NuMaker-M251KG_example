/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_drawing.cpp
 * @brief Free-hand drawing page application with color palette and low-pass filtering.
 * @details Implements an interactive 240x280 touch drawing canvas with a 14-color bottom toolbar,
 *          an exit button ("X") returning to the main menu, and first-order low-pass filtering (`Lpf`)
 *          on X and Y touch coordinates for smooth free-hand line rendering.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <Lpf/Lpf.h>
#include <page.h>

/**
 * @brief Registered task identifier for the interactive free-hand drawing page.
 */
int16_t pageDisplayDrawingId;

/**
 * @brief Index of the currently selected color swatch in `gPalette` (0 to 13).
 */
volatile uint32_t gLastSelectedPalette;

/**
 * @brief Default gray border color (`#808080`) drawn around unselected palette buttons.
 */
static Color gBasicBorderColor = {0x80, 0x80, 0x80};

/**
 * @brief Highlighted red border color (`#FF0000`) drawn around the currently selected palette button.
 */
static Color gSelectedBorderColor = {0xFF, 0x00, 0x00};

/**
 * @brief Low-pass filter instances (cutoff factor 20.0) for smoothing X and Y touch drag coordinates.
 */
static Lpf gLpfX(20.f), gLpfY(20.f);

/**
 * @brief Table of 14 selectable RGB888 brush colors arranged in two rows of 7 swatches.
 */
static Color gPalette[14] = 
{
	{0x00, 0x00, 0x00}, ///< Index 0: Black
	{0xFF, 0xFF, 0xFF}, ///< Index 1: White
	{0xFF, 0x00, 0x00}, ///< Index 2: Red
	{0x00, 0xFF, 0x00}, ///< Index 3: Green
	{0x00, 0x00, 0xFF}, ///< Index 4: Blue
	{0xFF, 0xFF, 0x00}, ///< Index 5: Yellow
	{0xFF, 0x00, 0xFF}, ///< Index 6: Magenta

	{0x00, 0xFF, 0xFF}, ///< Index 7: Cyan
	{0xFF, 0x55, 0x00}, ///< Index 8: Orange
	{0x80, 0x00, 0x80}, ///< Index 9: Purple
	{0xFF, 0xC0, 0xCB}, ///< Index 10: Pink
	{0xFF, 0xD7, 0x00}, ///< Index 11: Gold
	{0xC0, 0xC0, 0xC0}, ///< Index 12: Silver
	{0x00, 0x00, 0x80}, ///< Index 13: Navy
};

/**
 * @brief Screen bounding boxes (`Area`) corresponding to each of the 14 color palette buttons.
 */
static Area gPaletteArea[14];

/**
 * @details Initializes drawing canvas GUI components:
 *          1. Acquires the LCD lock and clears the screen with a white background.
 *          2. Initializes the 14 palette buttons via `initializePalette()` and selects black (index 0) as default.
 *          3. Renders the 30x40 Exit ("X") button to the right of the palette toolbar.
 *          4. Registers touch hit-box listeners for all 14 palette buttons, the drawing canvas region, and the exit button.
 *          5. Enters an infinite thread yield loop while touch callbacks handle user input.
 * @see initializePalette, callback_draw, callback_exit, dispatcher
 * @todo none
 * @bug none
 */
void pageDisplayDrawing(void*)
{
	// Acquire exclusive access to the LCD during initial page layout rendering
	lcd.lock();

	Area area;

	// Clear entire display background to white (forming the blank drawing canvas)
	lcd.setBackgroundColor(0xFF, 0xFF, 0xFF);
	lcd.clear();
	
	// Render the 14 color palette buttons and select palette 0 (Black) by default
	initializePalette();
	callback_Palette0({0, 0, PointerDevice::EVENT_TOUCH_DOWN});
	
	// Define the 30x40 "Exit" button bounding box immediately to the right of palette button 6
	area.setX(gPaletteArea[6].getRightBound());
	area.setY(gPaletteArea[6].getTopBound());
	area.setWidth(30);
	area.setHeight(40);

	// Configure scratchpad brush to match the 30x40 exit button dimensions and fill outer border gray
	brush.setSize(area);
	brush.setBackgroundColor(0x80, 0x80, 0x80);
	brush.clear();

	// Render lighter gray inner button face with a 3-pixel border margin
	brush.setBrushColor(0xA0, 0xA0, 0xA0);
	brush.fillRectangular({3, 3, 30 - 6, 40 - 6});
	
	// Draw centered "X" exit label in black using Ubuntu 20 Bold font
	brush.setFont(Font_Ubuntu_20_B);
	brush.setBrushColor(0x00, 0x00, 0x00);
	brush.drawString(Brush::ALIGN_CENTER_MID, "X");

	// Flush the rendered exit button bitmap to the LCD
	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
	
	// Release LCD mutex lock
	lcd.unlock();

	// Clear existing touch regions and register all 14 palette swatches, canvas area, and exit button
	dispatcher.clear();
	dispatcher.add(gPaletteArea[0], callback_Palette0);
	dispatcher.add(gPaletteArea[1], callback_Palette1);
	dispatcher.add(gPaletteArea[2], callback_Palette2);
	dispatcher.add(gPaletteArea[3], callback_Palette3);
	dispatcher.add(gPaletteArea[4], callback_Palette4);
	dispatcher.add(gPaletteArea[5], callback_Palette5);
	dispatcher.add(gPaletteArea[6], callback_Palette6);
	dispatcher.add(gPaletteArea[7], callback_Palette7);
	dispatcher.add(gPaletteArea[8], callback_Palette8);
	dispatcher.add(gPaletteArea[9], callback_Palette9);
	dispatcher.add(gPaletteArea[10], callback_Palette10);
	dispatcher.add(gPaletteArea[11], callback_Palette11);
	dispatcher.add(gPaletteArea[12], callback_Palette12);
	dispatcher.add(gPaletteArea[13], callback_Palette13);
	// Bind the upper canvas region above the palette toolbar to callback_draw
	dispatcher.add({0, 0, 239, (uint16_t)(gPaletteArea[0].getTopBound() - 1)}, callback_draw);
	// Bind the bottom-right exit button to callback_exit
	dispatcher.add(area, callback_exit);

	// Yield execution in an infinite loop while the touch event dispatcher handles drawing
	while(true)
	{
		thread::yield();
	}
}

/**
 * @details Handles touch down and drag events on the drawing canvas:
 *          - On `EVENT_TOUCH_DOWN`: records the starting coordinate in `lastEvent` and seeds the X/Y low-pass filters.
 *          - On `EVENT_TOUCH_DRAG`: filters the incoming raw touch coordinates through `gLpfX` and `gLpfY`,
 *            clamps coordinates within `[0..239, 0..319]`, draws a line segment from `lastEvent` to the new filtered
 *            point using the active palette color, and updates `lastEvent`.
 * @see gLpfX, gLpfY, gPalette, gLastSelectedPalette, Lpf::calculate
 * @todo none
 * @bug none
 */
void callback_draw(PointerDevice::event_t event)
{
	static PointerDevice::event_t lastEvent;
	int16_t x, y;

	switch(event.event)
	{
	case PointerDevice::EVENT_TOUCH_DOWN :
		// Store initial touch point and seed low-pass filters to prevent jump artifacts
		lastEvent = event;
		gLpfX.setCurrentData(event.x);
		gLpfY.setCurrentData(event.y);
		break;
	
	case PointerDevice::EVENT_TOUCH_DRAG :
		// Apply low-pass filtering to X coordinate and clamp to horizontal display bounds [0, 239]
		x = gLpfX.calculate(event.x);
		if(x < 0)
			x = 0;
		else if(x > 239)
			x = 239;
		// Apply low-pass filtering to Y coordinate and clamp to vertical display bounds [0, 319]
		y = gLpfY.calculate(event.y);
		if(y < 0)
			y = 0;
		else if(y > 319)
			y = 319;

		// Acquire LCD lock and draw a continuous line segment from the previous point to the smoothed point
		lcd.lock();
		lcd.setBrushColor(gPalette[gLastSelectedPalette]);
		lcd.drawLine({lastEvent.x, lastEvent.y}, {x, y});
		lcd.unlock();

		// Update previous coordinate for the next drag segment
		lastEvent.x = x;
		lastEvent.y = y;
		break;

	default :
		break;
	}
}

/**
 * @details Selects palette color 0 (Black) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette0(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 0 (Black) and draw highlighted border
		gLastSelectedPalette = 0;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 1 (White) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette1(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 1 (White) and draw highlighted border
		gLastSelectedPalette = 1;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 2 (Red) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette2(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 2 (Red) and draw highlighted border
		gLastSelectedPalette = 2;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 3 (Green) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette3(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 3 (Green) and draw highlighted border
		gLastSelectedPalette = 3;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 4 (Blue) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette4(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 4 (Blue) and draw highlighted border
		gLastSelectedPalette = 4;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 5 (Yellow) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette5(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 5 (Yellow) and draw highlighted border
		gLastSelectedPalette = 5;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 6 (Magenta) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette6(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 6 (Magenta) and draw highlighted border
		gLastSelectedPalette = 6;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 7 (Cyan) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette7(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 7 (Cyan) and draw highlighted border
		gLastSelectedPalette = 7;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 8 (Orange) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette8(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 8 (Orange) and draw highlighted border
		gLastSelectedPalette = 8;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 9 (Purple) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette9(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 9 (Purple) and draw highlighted border
		gLastSelectedPalette = 9;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 10 (Pink) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette10(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 10 (Pink) and draw highlighted border
		gLastSelectedPalette = 10;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 11 (Gold) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette11(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 11 (Gold) and draw highlighted border
		gLastSelectedPalette = 11;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 12 (Silver) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette12(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 12 (Silver) and draw highlighted border
		gLastSelectedPalette = 12;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 13 (Navy) and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette13(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 13 (Navy) and draw highlighted border
		gLastSelectedPalette = 13;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 14 if defined and updates the palette button border highlights.
 * @see drawPalette, gLastSelectedPalette, gPalette
 * @todo none
 * @bug none
 */
void callback_Palette14(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		// Restore unselected border on previously active palette button
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		// Set active palette index to 14
		gLastSelectedPalette = 14;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Callback invoked when the Exit ("X") button is touched. Switches the current
 *          GUI task back to the main menu page (`pageDisplayMainId`).
 * @see page, pageDisplayMainId, TaskManager::play
 * @todo none
 * @bug none
 */
void callback_exit(PointerDevice::event_t event)
{
	// Return to the main menu page when the Exit button is pressed down
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		page.play(pageDisplayMainId);
	}
}

/**
 * @details Calculates screen positions for two rows of 7 palette buttons (30x20 pixels each)
 *          at the bottom of the 240x320 screen, saves their hit areas in `gPaletteArea`,
 *          and renders the initial color swatches on the LCD.
 * @see drawPaletteTop, drawPaletteBot, gPaletteArea, gPalette
 * @todo none
 * @bug none
 */
void initializePalette()
{
	Area area;

	// Configure each palette button cell size to 30x20 pixels
	area.setWidth(30);
	area.setHeight(20);
	brush.setSize(area);
	brush.setBackgroundColor({0x80, 0x80, 0x80});
	
	// Render top row of palette buttons (indices 0 to 6) at Y = 279
	area.setY(319-40);
	area.setX(0);

	for(uint32_t i = 0; i < 7; i++)
	{
		drawPaletteTop(area, gBasicBorderColor, gPalette[i]);
		gPaletteArea[i] = area;
		area.addX(area.getWidth());
	}

	// Render bottom row of palette buttons (indices 7 to 13) at Y = 299
	area.setY(319-20);
	area.setX(0);

	for(uint32_t i = 0; i < 7; i++)
	{
		drawPaletteBot(area, gBasicBorderColor, gPalette[i+7]);
		gPaletteArea[i+7] = area;
		area.addX(area.getWidth());
	}
}

/**
 * @details Draws a top-row palette swatch button with outer border color `bdColor` and inner fill `color`.
 * @see brush, lcd, drawPalette
 * @todo none
 * @bug none
 */
void drawPaletteTop(Area area, Color bdColor, Color color)
{
	// Fill background with border color and draw inset 26x14 swatch rectangle at offset (2, 4)
	brush.setBackgroundColor(bdColor);
	brush.setBrushColor(color);
	brush.clear();
	brush.fillRectangular({2, 4 , 26, 14});
	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
}

/**
 * @details Draws a bottom-row palette swatch button with outer border color `bdColor` and inner fill `color`.
 * @see brush, lcd, drawPalette
 * @todo none
 * @bug none
 */
void drawPaletteBot(Area area, Color bdColor, Color color)
{
	// Fill background with border color and draw inset 26x14 swatch rectangle at offset (2, 2)
	brush.setBackgroundColor(bdColor);
	brush.setBrushColor(color);
	brush.clear();
	brush.fillRectangular({2, 2 , 26, 14});
	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
}

/**
 * @details Renders a palette button at the given index with specified border and fill color.
 *          Dispatches drawing to `drawPaletteTop()` for indices 0..6 or `drawPaletteBot()` for indices 7..13.
 * @see drawPaletteTop, drawPaletteBot, gPaletteArea
 * @todo none
 * @bug none
 */
void drawPalette(uint32_t index, Color bdColor, Color color)
{
	// Route to top row (0..6) or bottom row (7..13) swatch renderer
	if(index < 7)
		drawPaletteTop(gPaletteArea[index], bdColor, color);
	else if(index < 14)
		drawPaletteBot(gPaletteArea[index], bdColor, color);		
}


