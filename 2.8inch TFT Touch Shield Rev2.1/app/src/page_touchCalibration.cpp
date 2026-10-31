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

enum step_t
{
	STEP_WAITING_PRESS_P1,
	STEP_CALIBRATING_P1,
	STEP_WAITING_UP_P1,
	STEP_WAITING_PRESS_P2,
	STEP_CALIBRATING_P2,
	STEP_WAITING_UP_P2,
};

int16_t pageDisplayTouchCalibrationId;

ElapsedTime gLastUpdateTime;
Lpf gLpfX(2.f), gLpfY(2.f);
volatile bool gPushFlag, gUpFlag, gCalibratingFlag, gFirstContactFlag;

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
				gFirstContactFlag = false;
				gLpfX.setCurrentData(touch.getLastConvertedAdcX());
				gLpfY.setCurrentData(touch.getLastConvertedAdcY());				
			}
			else
			{
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

void pageDisplayTouchCalibration(void*)
{
	ProgressDisplay progressBar;
	step_t step = STEP_WAITING_PRESS_P1;
	ElapsedTime pressTime;
	int32_t buf;
	char str[32];

	progressBar.setFrameBuffer(brush);
	progressBar.setBgColor(0x30, 0x30, 0x30);
	progressBar.setColor({0x30, 0xFF, 0x30}, {0x80, 0x80, 0x80});
	progressBar.setArea({50, 200, 140, 30});
	progressBar.setLcd(lcd);

	lcd.lock();

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

	brush.setSize(35,35);
	brush.setBackgroundColor({0x00, 0x00, 0x00});
	brush.clear();
	brush.setBrushColor(0x80, 0x80, 0x80);
	brush.fillCircle({17, 17}, 16);
	brush.setBrushColor(0xFF, 0x30, 0x30);
	brush.fillCircle({17, 17}, 5);
	lcd.drawBitmap({(int16_t)(48 - brush.getSize().getWidth() / 2), (int16_t)(48 - brush.getSize().getHeight() / 2)}, brush.getBitmap());

	progressBar.drawBackground();
	progressBar.drawValue(0);

	lcd.unlock();

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

				sprintf(str, "%4d, %4d", (uint32_t)gLpfX.getCurrentData(), (uint32_t)gLpfY.getCurrentData());
				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.setBrushColor(0xFF, 0xFF, 0x30);
				brush.clear();
				brush.drawString(Brush::ALIGN_RIGHT_MID, str);
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());

				lcd.unlock();

				if(buf > 5000)
				{
					step = STEP_WAITING_UP_P1;
					gCalibratingFlag = false;

					lcd.lock();
					progressBar.drawValue(0);
					brush.setSize(35,35);
					brush.setBackgroundColor({0x00, 0x00, 0x00});
					brush.clear();
					lcd.drawBitmap({(int16_t)(48 - brush.getSize().getWidth() / 2), (int16_t)(48 - brush.getSize().getHeight() / 2)}, brush.getBitmap());
					lcd.unlock();
				}
			}
			else
			{
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

				lcd.lock();
				brush.setSize(35,35);
				brush.setBackgroundColor({0x00, 0x00, 0x00});
				brush.clear();
				brush.setBrushColor(0x80, 0x80, 0x80);
				brush.fillCircle({17, 17}, 16);
				brush.setBrushColor(0xFF, 0x30, 0x30);
				brush.fillCircle({17, 17}, 5);
				lcd.drawBitmap({(int16_t)(192 - brush.getSize().getWidth() / 2), (int16_t)(272 - brush.getSize().getHeight() / 2)}, brush.getBitmap());

				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.clear();
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());
				lcd.unlock();

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

				sprintf(str, "%4d, %4d", (uint32_t)gLpfX.getCurrentData(), (uint32_t)gLpfY.getCurrentData());
				brush.setSize(150, 15);
				brush.setBackgroundColor(0x00, 0x00, 0x00);
				brush.setBrushColor(0xFF, 0xFF, 0x30);
				brush.clear();
				brush.drawString(Brush::ALIGN_RIGHT_MID, str);
				lcd.drawBitmap({(int16_t)(239 - brush.getSize().getWidth()), 0}, brush.getBitmap());
				lcd.unlock();

				if(buf > 5000)
				{
					step = STEP_WAITING_UP_P2;
					gCalibratingFlag = false;

					lcd.lock();
					progressBar.drawValue(0);
					brush.setSize(35,35);
					brush.setBackgroundColor({0x00, 0x00, 0x00});
					brush.clear();
					lcd.drawBitmap({(int16_t)(192 - brush.getSize().getWidth() / 2), (int16_t)(272 - brush.getSize().getHeight() / 2)}, brush.getBitmap());
					lcd.unlock();
					thread::delay(100);
				}
			}
			else
			{
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

				memory.touch.botRightX = gLpfX.getCurrentData();
				memory.touch.botRightY = gLpfY.getCurrentData();
				memory.touchInitFlag = true;

				saveMemory();
				updateMemory();

				page.play(pageDisplayMainId);
				while(1)
					thread::yield();
			}
			break;
		}
		thread::yield();
	}
}

