/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_logo.cpp
 * @brief Startup splash screen displaying Nuvoton logo and system information.
 * @details Renders the Nuvoton corporate logo bitmap, evaluation board title, shield description,
 *          and a blinking "Example" banner using the `FrameBufferRgb565LE` scratchpad brush.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>

/**
 * @brief Registered task identifier for the startup logo splash screen.
 */
int16_t pageDisplayLogoId;

/**
 * @details Renders the startup splash page on the 2.8-inch TFT LCD:
 *          1. Acquires the LCD mutex lock (`lcd.lock()`).
 *          2. Draws the Nuvoton corporate bitmap logo (`NuvotonLogo`) at coordinate (20, 30).
 *          3. Displays centered title strings `"NuMaker-M251KG V1.1"`, `"with"`, `"2.8inch TFT Touch Shield"`,
 *             and `"Powered by yss OS"` using `Font_Ubuntu_14_B`.
 *          4. Draws `"Example"` using `Font_Ubuntu_20_B` at (20, 200) and releases the LCD lock.
 *          5. Enters an infinite animation loop toggling the `"Example"` banner every 250 milliseconds.
 * @see lcd, brush, NuvotonLogo, Font_Ubuntu_14_B, Font_Ubuntu_20_B
 * @todo none
 * @bug none
 */
void pageDisplayLogo(void*)
{
	// Acquire exclusive access to the SPI-connected LCD controller
	lcd.lock();

	// Draw 200x25 Nuvoton company logo bitmap at top-center (20, 30)
	lcd.drawBitmap({20, 30}, NuvotonLogo);

	// Configure scratchpad frame buffer brush (240x20 pixels, black text on white background)
	brush.setSize(240, 20);
	brush.setBackgroundColor({0xFF, 0xFF, 0xFF});
	brush.setBrushColor({0x00, 0x00, 0x00});
	brush.setFont(Font_Ubuntu_14_B);

	// Line 1: Render target evaluation board model name at Y = 100
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "NuMaker-M251KG V1.1");
	lcd.drawBitmap({0, 100}, brush.getBitmap());

	// Line 2: Render connector word "with" at Y = 125
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "with");
	lcd.drawBitmap({0, 125}, brush.getBitmap());

	// Line 3: Render shield accessory name at Y = 150
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "2.8inch TFT Touch Shield");
	lcd.drawBitmap({0, 150}, brush.getBitmap());

	// Line 4: Render OS branding footer near the bottom of the screen at Y = 280
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Powered by yss OS");
	lcd.drawBitmap({0, 280}, brush.getBitmap());

	// Configure larger 20pt bold font for the blinking "Example" banner (200x25 pixels)
	brush.setFont(Font_Ubuntu_20_B);
	brush.setSize(200, 25);

	// Render initial visible state of the "Example" banner at (20, 200)
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Example");
	lcd.drawBitmap({20, 200}, brush.getBitmap());

	// Release LCD mutex lock after static splash elements are drawn
	lcd.unlock();

	// Blinking animation loop: alternates clearing and redrawing "Example" every 250 ms
	while(true)
	{
		// Wait 250 ms and erase the "Example" banner area to white
		thread::delay(250);
		lcd.lock();
		brush.clear();
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();

		// Wait 250 ms and redraw the "Example" banner text
		thread::delay(250);
		lcd.lock();
		brush.clear();
		brush.drawString(Brush::ALIGN_CENTER_MID, "Example");
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();
	}
}


