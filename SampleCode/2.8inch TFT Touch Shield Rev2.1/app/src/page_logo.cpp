/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_logo.cpp
 * @brief Startup splash screen displaying Nuvoton logo and system information.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>

int16_t pageDisplayLogoId;

/**
 * @details Renders the startup splash page on the 2.8-inch TFT LCD:
 *          1. Draws the Nuvoton corporate bitmap logo at coordinate (20, 30).
 *          2. Displays title texts "NuMaker-M251KG V1.1", "with", "2.8inch TFT Touch Shield",
 *             and "Powered by yss OS" with Ubuntu 14 Bold font.
 *          3. Enters a blinking loop toggling the "Example" banner every 250 milliseconds.
 * @see lcd, brush, NuvotonLogo, Font_Ubuntu_14_B, Font_Ubuntu_20_B
 * @todo none
 * @bug none
 */
void pageDisplayLogo(void*)
{
	lcd.lock();

	// Draw Nuvoton company logo bitmap
	lcd.drawBitmap({20, 30}, NuvotonLogo);

	// Configure brush for title descriptions
	brush.setSize(240, 20);
	brush.setBackgroundColor({0xFF, 0xFF, 0xFF});
	brush.setBrushColor({0x00, 0x00, 0x00});
	brush.setFont(Font_Ubuntu_14_B);

	// Line 1: Board model name
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "NuMaker-M251KG V1.1");
	lcd.drawBitmap({0, 100}, brush.getBitmap());

	// Line 2: Connector word
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "with");
	lcd.drawBitmap({0, 125}, brush.getBitmap());

	// Line 3: Shield accessory name
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "2.8inch TFT Touch Shield");
	lcd.drawBitmap({0, 150}, brush.getBitmap());

	// Line 4: OS branding footer
	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Powered by yss OS");
	lcd.drawBitmap({0, 280}, brush.getBitmap());

	// Draw blinking "Example" text banner
	brush.setFont(Font_Ubuntu_20_B);
	brush.setSize(200, 25);

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Example");
	lcd.drawBitmap({20, 200}, brush.getBitmap());

	lcd.unlock();

	// Blinking animation loop: alternates display every 250ms
	while(true)
	{
		thread::delay(250);
		lcd.lock();
		brush.clear();
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();

		thread::delay(250);
		lcd.lock();
		brush.clear();
		brush.drawString(Brush::ALIGN_CENTER_MID, "Example");
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();
	}
}


