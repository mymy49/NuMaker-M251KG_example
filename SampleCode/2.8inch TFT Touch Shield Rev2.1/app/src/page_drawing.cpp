/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_drawing.cpp
 * @brief Free-hand drawing page application with color palette and low-pass filtering.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <Lpf/Lpf.h>
#include <page.h>

int16_t pageDisplayDrawingId;

volatile uint32_t gLastSelectedPalette;

static Color gBasicBorderColor = {0x80, 0x80, 0x80};

static Color gSelectedBorderColor = {0xFF, 0x00, 0x00};

static Lpf gLpfX(20.f), gLpfY(20.f);

static Color gPalette[14] = 
{
	{0x00, 0x00, 0x00},
	{0xFF, 0xFF, 0xFF},
	{0xFF, 0x00, 0x00},
	{0x00, 0xFF, 0x00},
	{0x00, 0x00, 0xFF},
	{0xFF, 0xFF, 0x00},
	{0xFF, 0x00, 0xFF},

	{0x00, 0xFF, 0xFF},
	{0xFF, 0x55, 0x00},
	{0x80, 0x00, 0x80},
	{0xFF, 0xC0, 0xCB},
	{0xFF, 0xD7, 0x00},
	{0xC0, 0xC0, 0xC0},
	{0x00, 0x00, 0x80},
};

static Area gPaletteArea[14];

void drawPaletteTop(Area area, Color bdColor, Color color);

void drawPaletteBot(Area area, Color bdColor, Color color);

void drawPalette(uint32_t index, Color bdColor, Color color);

void initializePalette();

void callback_Palette0(PointerDevice::event_t event);

void callback_Palette1(PointerDevice::event_t event);

void callback_Palette2(PointerDevice::event_t event);

void callback_Palette3(PointerDevice::event_t event);

void callback_Palette4(PointerDevice::event_t event);

void callback_Palette5(PointerDevice::event_t event);

void callback_Palette6(PointerDevice::event_t event);

void callback_Palette7(PointerDevice::event_t event);

void callback_Palette8(PointerDevice::event_t event);

void callback_Palette9(PointerDevice::event_t event);

void callback_Palette10(PointerDevice::event_t event);

void callback_Palette11(PointerDevice::event_t event);

void callback_Palette12(PointerDevice::event_t event);

void callback_Palette13(PointerDevice::event_t event);

void callback_draw(PointerDevice::event_t event);

void callback_exit(PointerDevice::event_t event);

/**
 * @details Initializes drawing canvas GUI components:
 *          1. Clears LCD screen with white background.
 *          2. Initializes palette buttons and sets default color to black (palette index 0).
 *          3. Renders the exit ("X") button at the right of the palette toolbar.
 *          4. Registers touch listeners for drawing region, palette buttons, and exit button.
 *          5. Enters an infinite thread yield loop waiting for touch events.
 * @see initializePalette, callback_draw, callback_exit, dispatcher
 * @todo none
 * @bug none
 */
void pageDisplayDrawing(void*)
{
	lcd.lock();

	Area area;

	// Clear background to white
	lcd.setBackgroundColor(0xFF, 0xFF, 0xFF);
	lcd.clear();
	
	// Setup color palette buttons
	initializePalette();
	callback_Palette0({0, 0, PointerDevice::EVENT_TOUCH_DOWN});
	
	// Create "Exit" button area adjacent to the palette buttons
	area.setX(gPaletteArea[6].getRightBound());
	area.setY(gPaletteArea[6].getTopBound());
	area.setWidth(30);
	area.setHeight(40);

	brush.setSize(area);
	brush.setBackgroundColor(0x80, 0x80, 0x80);
	brush.clear();

	// Render inner button face
	brush.setBrushColor(0xA0, 0xA0, 0xA0);
	brush.fillRectangular({3, 3, 30 - 6, 40 - 6});
	
	// Draw exit label "X"
	brush.setFont(Font_Ubuntu_20_B);
	brush.setBrushColor(0x00, 0x00, 0x00);
	brush.drawString(Brush::ALIGN_CENTER_MID, "X");

	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
	
	lcd.unlock();

	// Register touch handlers to event dispatcher
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
	dispatcher.add({0, 0, 239, (uint16_t)(gPaletteArea[0].getTopBound() - 1)}, callback_draw);
	dispatcher.add(area, callback_exit);

	while(true)
	{
		thread::yield();
	}
}

/**
 * @details Handles touch drag and down events on the drawing canvas.
 *          Applies a low-pass filter (Lpf) on X and Y coordinates to smooth jitter.
 *          Draws lines between successive coordinate points using the currently selected palette color.
 * @see gLpfX, gLpfY, gPalette, gLastSelectedPalette
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
		// Seed the low pass filter with initial touch position
		lastEvent = event;
		gLpfX.setCurrentData(event.x);
		gLpfY.setCurrentData(event.y);
		break;
	
	case PointerDevice::EVENT_TOUCH_DRAG :
		// Calculate smoothed coordinates
		x = gLpfX.calculate(event.x);
		if(x < 0)
			x = 0;
		else if(x > 239)
			x = 239;
		y = gLpfY.calculate(event.y);
		if(y < 0)
			y = 0;
		else if(y > 319)
			y = 319;

		// Draw smooth line segment on LCD canvas
		lcd.lock();
		lcd.setBrushColor(gPalette[gLastSelectedPalette]);
		lcd.drawLine({lastEvent.x, lastEvent.y}, {x, y});
		lcd.unlock();
		lastEvent.x = x;
		lastEvent.y = y;
		break;

	default :
		break;
	}
}

/**
 * @details Selects palette color 0 (Black), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette0(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 0;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 1 (White), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette1(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 1;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 2 (Red), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette2(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 2;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 3 (Green), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette3(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 3;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 4 (Blue), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette4(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 4;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 5 (Yellow), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette5(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 5;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 6 (Magenta), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette6(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 6;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 7 (Cyan), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette7(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 7;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 8 (Orange), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette8(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 8;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 9 (Purple), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette9(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 9;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 10 (Pink), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette10(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 10;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 11 (Gold), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette11(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 11;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 12 (Silver), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette12(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 12;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 13 (Navy), updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette13(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 13;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Selects palette color 14 if defined, updates border highlights.
 * @see drawPalette
 * @todo none
 * @bug none
 */
void callback_Palette14(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		drawPalette(gLastSelectedPalette, gBasicBorderColor, gPalette[gLastSelectedPalette]);
		gLastSelectedPalette = 14;
		drawPalette(gLastSelectedPalette, gSelectedBorderColor, gPalette[gLastSelectedPalette]);
	}
}

/**
 * @details Callback invoked when the Exit button is touched. Switches the current
 *          page back to the main menu page.
 * @see page, pageDisplayMainId
 * @todo none
 * @bug none
 */
void callback_exit(PointerDevice::event_t event)
{
	if(event.event == PointerDevice::EVENT_TOUCH_DOWN)
	{
		page.play(pageDisplayMainId);
	}
}

/**
 * @details Calculates screen positions for two rows of palette buttons (7 items each)
 *          and renders the initial color swatches on the LCD.
 * @see drawPaletteTop, drawPaletteBot, gPaletteArea
 * @todo none
 * @bug none
 */
void initializePalette()
{
	Area area;

	area.setWidth(30);
	area.setHeight(20);
	brush.setSize(area);
	brush.setBackgroundColor({0x80, 0x80, 0x80});
	
	// Top row of palette buttons (indices 0 to 6)
	area.setY(319-40);
	area.setX(0);

	for(uint32_t i = 0; i < 7; i++)
	{
		drawPaletteTop(area, gBasicBorderColor, gPalette[i]);
		gPaletteArea[i] = area;
		area.addX(area.getWidth());
	}

	// Bottom row of palette buttons (indices 7 to 13)
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
 * @details Draws a top-row palette swatch button with outer border and inner color fill.
 * @see brush, lcd
 * @todo none
 * @bug none
 */
void drawPaletteTop(Area area, Color bdColor, Color color)
{
	brush.setBackgroundColor(bdColor);
	brush.setBrushColor(color);
	brush.clear();
	brush.fillRectangular({2, 4 , 26, 14});
	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
}

/**
 * @details Draws a bottom-row palette swatch button with outer border and inner color fill.
 * @see brush, lcd
 * @todo none
 * @bug none
 */
void drawPaletteBot(Area area, Color bdColor, Color color)
{
	brush.setBackgroundColor(bdColor);
	brush.setBrushColor(color);
	brush.clear();
	brush.fillRectangular({2, 2 , 26, 14});
	lcd.drawBitmap(area.getPosition(), brush.getBitmap());
}

/**
 * @details Renders a palette button at the given index with specified border and color.
 *          Directs drawing to drawPaletteTop or drawPaletteBot based on index range.
 * @see drawPaletteTop, drawPaletteBot
 * @todo none
 * @bug none
 */
void drawPalette(uint32_t index, Color bdColor, Color color)
{
	if(index < 7)
		drawPaletteTop(gPaletteArea[index], bdColor, color);
	else if(index < 14)
		drawPaletteBot(gPaletteArea[index], bdColor, color);		
}


