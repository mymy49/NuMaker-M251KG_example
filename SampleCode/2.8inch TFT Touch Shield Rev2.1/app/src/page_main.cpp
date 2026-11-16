#include <bsp.h>
#include "../bmp/drawing.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>
#include <page.h>

int16_t pageDisplayMainId;

static void callback_drawing(PointerDevice::event_t event)
{
	switch(event.event)
	{
	case PointerDevice::EVENT_TOUCH_DOWN :
		page.play(pageDisplayDrawingId);
		break;

	case PointerDevice::EVENT_TOUCH_UP :
		break;
	
	default :	
		break;
	}
}

void pageDisplayMain(void*)
{
	lcd.lock();

	lcd.setBackgroundColor({0x00, 0x00, 0x00});
	lcd.clear();
	
	lcd.drawBitmap({20, 20}, drawing);

	lcd.unlock();
		
	dispatcher.clear();
	dispatcher.add({20, 20, 90, 90}, callback_drawing);

	while(true)
	{
		thread::yield();
	}
}

