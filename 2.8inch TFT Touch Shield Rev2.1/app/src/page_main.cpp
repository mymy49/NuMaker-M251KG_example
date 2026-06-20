#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>

int16_t pageDisplayMainId;

void pageDisplayMain(void*)
{
	lcd.lock();

	lcd.setBackgroundColor({0x00, 0x00, 0x00});
	lcd.clear();

	lcd.unlock();
		
	while(true)
	{
		thread::yield();
	}
}

