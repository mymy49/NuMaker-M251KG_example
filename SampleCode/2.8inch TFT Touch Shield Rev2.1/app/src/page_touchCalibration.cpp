/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page_touchCalibration.cpp
 * @brief Resistive touch screen 2-point calibration routine with flash persistence.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <ValueDisplay/ProgressDisplay.h>
#include <util/ElapsedTime.h>
#include <Lpf/Lpf.h>
#include <yss/debug.h>
#include <stdio.h>
#include <memory.h>
#include <page.h>

/// Calibration state machine steps
enum step_t
{
	STEP_WAITING_PRESS_P1,  ///< Waiting for initial touch on Point 1 (Top-Left)
	STEP_CALIBRATING_P1,    ///< Filtering raw ADC readings during Point 1 press
	STEP_WAITING_UP_P1,     ///< Waiting for finger release from Point 1
	STEP_WAITING_PRESS_P2,  ///< Waiting for initial touch on Point 2 (Bottom-Right)
	STEP_CALIBRATING_P2,    ///< Filtering raw ADC readings during Point 2 press
	STEP_WAITING_UP_P2,     ///< Waiting for finger release from Point 2
};

int16_t pageDisplayTouchCalibrationId;

ElapsedTime gLastUpdateTime;
static Lpf gLpfX(2.f), gLpfY(2.f);
volatile bool gPushFlag, gUpFlag, gCalibratingFlag, gFirstContactFlag;

/**
 * @details Handles touch events during calibration:
 *          - EVENT_TOUCH_DOWN: flags touch press and resets low-pass filter state.
 *          - EVENT_TOUCH_DRAG: feeds raw ADC conversion values into low-pass filters (gLpfX, gLpfY).
 *          - EVENT_TOUCH_UP: flags release event to advance the state machine.
 * @see touch, gLpfX, gLpfY
 * @todo none
 * @bug none
 */
void callback_calibrate(PointerDevice::event_t event)
{
	gLastUpdateTime.reset();

	switch(event.event)
	{
	case PointerDevice::EVENT_TOUCH_DOWN :
		gPushFlag = true;
		gUpFlag = false;
		gFirstContactFlag = true;
		break;
	
	case PointerDevice::EVENT_TOUCH_DRAG :
		if(gCalibratingFlag)
		{
			if(gFirstContactFlag)
			{
				// Seed filters with initial raw touch ADC coordinates
				gFirstContactFlag = false;
				gLpfX.setCurrentData(touch.getLastConvertedAdcX());
				gLpfY.setCurrentData(touch.getLastConvertedAdcY());				
			}
			else
			{
				// Update moving average filter with incoming ADC samples
				gLpfX.calculate(touch.getLastConvertedAdcX());
				gLpfY.calculate(touch.getLastConvertedAdcY());
			}
		}
		break;

	case PointerDevice::EVENT_TOUCH_UP :
		gUpFlag = true;
		break;

	default :
		break;
	}
}

/**
 * @details Runs the interactive 2-point touch screen calibration state machine:
 *          1. Prompts user to press the top-left red target point until the progress bar reaches 100%.
 *          2. Records the filtered ADC values for top-left (P1).
 *          3. Prompts user to press the bottom-right red target point until completion.
 *          4. Records the filtered ADC values for bottom-right (P2).
 *          5. Saves calibration matrix into flash memory via saveMemory(), updates runtime driver,
 *             and transitions to the main page.
 * @see callback_calibrate, saveMemory, updateMemory, pageDisplayMainId
 * @todo none
 * @bug none
 */
void pageDisplayTouchCalibration(void*)
{
	ProgressDisplay progressBar;
	step_t step = STEP_WAITING_PRESS_P1;
	ElapsedTime pressTime;
	int32_t buf;
	char str[32];

	// Initialize on-screen progress bar
	progressBar.setFrameBuffer(brush);
	progressBar.setBgColor(0x30, 0x30, 0x30);
	progressBar.setColor({0x30, 0xFF, 0x30}, {0x80, 0x80, 0x80});
	progressBar.setArea({50, 200, 140, 30});
	progressBar.setLcd(lcd);

	lcd.lock();

	// Render background and calibration instructions
	lcd.setBackgroundColor({0x00, 0x00, 0x00});
	lcd.clear();

	brush.setSize(210, 23);

	brush.setFont(Font_Ubuntu_20_B);
	brush.setBackgroundColor({0x00, 0x00, 0x00});
	brush.setBrushColor({0xFF, 0xFF, 0x30});

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "[Touch Calibration]");
	lcd.drawBitmap({15, 100}, brush.getBitmap());

	brush.setFont(Font_Ubuntu_14_B);
	brush.setBackgroundColor({0x00, 0x00, 0x00});
	brush.setBrushColor({0xFF, 0xFF, 0xFF});

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Press Red Point");
	lcd.drawBitmap({15, 140}, brush.getBitmap());

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Until Complete.");
	lcd.drawBitmap({15, 165}, brush.getBitmap());

	// Draw top-left calibration bullseye target (P1)
	brush.setSize(35,35);
	brush.setBackgroundColor({0x00, 0x00, 0x00});
	brush.clear();
	brush.setBrushColor(0x80, 0x80, 0x80);
	brush.fillCircle({5, 5}, 16);
	brush.setBrushColor(0xFF, 0x30, 0x30);
	brush.fillCircle({5, 5}, 5);
	lcd.drawBitmap({0, 0}, brush.getBitmap());

	progressBar.drawBackground();
	progressBar.drawValue(0);

	lcd.unlock();

	// Bind entire screen to touch calibration listener
	dispatcher.clear();
	dispatcher.add({0, 0, 240, 320}, callback_calibrate);

	while(true)
	{
		switch(step)
		{
		case STEP_WAITING_PRESS_P1 :
			if(gPushFlag)
			{
				gPushFlag = false;
				gCalibratingFlag = true;
				step = STEP_CALIBRATING_P1;
				pressTime.reset();
			}
			break;
		
		case STEP_CALIBRATING_P1 :
			if(!gUpFlag)
			{
				buf = (int32_t)pressTime.getMsec();
				lcd.lock();
				progressBar.drawValue(buf / 50);

				// Display live raw ADC coordinates
				sprintf(str, "%4d, %4d", (uint32_t)gLpfX.getCurrentData(), (uint32_t)gLpfY.getCurrentData());
				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.setBrushColor(0xFF, 0xFF, 0x30);
				brush.clear();
				brush.drawString(Brush::ALIGN_RIGHT_MID, str);
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());

				lcd.unlock();

				// Calibration hold complete after 5000ms
				if(buf > 5000)
				{
					step = STEP_WAITING_UP_P1;
					gCalibratingFlag = false;

					// Clear target P1 from top-left
					lcd.lock();
					progressBar.drawValue(0);
					brush.setSize(35,35);
					brush.setBackgroundColor({0x00, 0x00, 0x00});
					brush.clear();
					lcd.drawBitmap({0, 0}, brush.getBitmap());
					lcd.unlock();
				}
			}
			else
			{
				// User released early: reset P1 step
				gUpFlag = false;
				gCalibratingFlag = false;
				step = STEP_WAITING_PRESS_P1;

				lcd.lock();
				progressBar.drawValue(0);
				lcd.unlock();
			}
			break;
		
		case STEP_WAITING_UP_P1 :
			if(gUpFlag)
			{
				gUpFlag = false;
				step = STEP_WAITING_PRESS_P2;

				// Draw bottom-right target bullseye (P2)
				lcd.lock();
				brush.setSize(35,35);
				brush.setBackgroundColor({0x00, 0x00, 0x00});
				brush.clear();
				brush.setBrushColor(0x80, 0x80, 0x80);
				brush.fillCircle({29, 29}, 16);
				brush.setBrushColor(0xFF, 0x30, 0x30);
				brush.fillCircle({29, 29}, 5);
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), (int16_t)(319 - brush.getSize().getHeight())}, brush.getBitmap());

				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.clear();
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());
				lcd.unlock();

				// Store filtered P1 coordinates
				memory.touch.topLeftX = gLpfX.getCurrentData();
				memory.touch.topLeftY = gLpfY.getCurrentData();
			}
			break;

		case STEP_WAITING_PRESS_P2 :
			if(gPushFlag)
			{
				gPushFlag = false;
				gCalibratingFlag = true;
				step = STEP_CALIBRATING_P2;
				pressTime.reset();
			}
			break;

		case STEP_CALIBRATING_P2 :
			if(!gUpFlag)
			{
				buf = (int32_t)pressTime.getMsec();
				lcd.lock();
				progressBar.drawValue(buf / 50);

				// Display live raw ADC coordinates
				sprintf(str, "%4d, %4d", (uint32_t)gLpfX.getCurrentData(), (uint32_t)gLpfY.getCurrentData());
				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.setBrushColor(0xFF, 0xFF, 0x30);
				brush.clear();
				brush.drawString(Brush::ALIGN_RIGHT_MID, str);
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());
				lcd.unlock();

				// Calibration hold complete after 5000ms
				if(buf > 5000)
				{
					step = STEP_WAITING_UP_P2;
					gCalibratingFlag = false;

					// Clear target P2 from bottom-right
					lcd.lock();
					progressBar.drawValue(0);
					brush.setSize(35,35);
					brush.setBackgroundColor({0x00, 0x00, 0x00});
					brush.clear();
					lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), (int16_t)(319 - brush.getSize().getHeight())}, brush.getBitmap());
					lcd.unlock();
					thread::delay(100);
				}
			}
			else
			{
				// User released early: reset P2 step
				gUpFlag = false;
				step = STEP_WAITING_PRESS_P2;
				gCalibratingFlag = false;

				lcd.lock();
				progressBar.drawValue(0);
				lcd.unlock();
			}
			break;

		case STEP_WAITING_UP_P2 :
			if(gUpFlag)
			{
				gUpFlag = false;
				
				lcd.lock();
				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.clear();
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());
				lcd.unlock();

				// Store filtered P2 coordinates and mark calibration complete
				memory.touch.botRightX = gLpfX.getCurrentData();
				memory.touch.botRightY = gLpfY.getCurrentData();
				memory.touchInitFlag = true;

				// Save calibration data to flash memory and update hardware driver
				saveMemory();
				updateMemory();

				// Advance to main menu
				page.play(pageDisplayMainId);
				while(1)
					thread::yield();
			}
			break;
		}
		thread::yield();
	}
}


