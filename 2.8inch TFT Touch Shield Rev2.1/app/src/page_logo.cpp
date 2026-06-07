#include <bsp.h>
#include "../bmp/NuvotonLogo.h"
#include <../font/Ubuntu_14_B.h>
#include <../font/Ubuntu_20_B.h>

int16_t pageDisplayLogoId;

void pageDisplayLogo(void*)
{
	lcd.lock();
	lcd.drawBitmap({20, 30}, NuvotonLogo);

	brush.setSize(240, 20);
	brush.setBackgroundColor({0xFF, 0xFF, 0xFF});
	brush.setBrushColor({0x00, 0x00, 0x00});
	brush.setFont(Font_Ubuntu_14_B);

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "NuMaker-M433SE V1.0");
	lcd.drawBitmap({0, 100}, brush.getBitmap());

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "with");
	lcd.drawBitmap({0, 125}, brush.getBitmap());

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "2.8inch TFT Touch Shield");
	lcd.drawBitmap({0, 150}, brush.getBitmap());

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Powered by yss OS");
	lcd.drawBitmap({0, 280}, brush.getBitmap());

	brush.setFont(Font_Ubuntu_20_B);
	brush.setSize(200, 25);

	brush.clear();
	brush.drawString(Brush::ALIGN_CENTER_MID, "Exameple");
	lcd.drawBitmap({20, 200}, brush.getBitmap());

	lcd.unlock();
		
	while(true)
	{
		thread::delay(250);
		brush.clear();
		lcd.lock();
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();

		thread::delay(250);
		brush.clear();
		brush.drawString(Brush::ALIGN_CENTER_MID, "Exameple");
		lcd.lock();
		lcd.drawBitmap({20, 200}, brush.getBitmap());
		lcd.unlock();
	}
}

