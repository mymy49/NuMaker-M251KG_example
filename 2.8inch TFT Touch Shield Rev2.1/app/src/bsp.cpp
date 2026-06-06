/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

Touch_LCD_Shield_for_Arduino_2_8_inch lcd;

FrameBufferRgb565LE brush;

void initializeBoard(void)
{
	// SPI0
	gpioA.setAsAltFunc(0, Gpio::PA0_SPI0_MOSI);
	gpioA.setAsAltFunc(1, Gpio::PA1_SPI0_MISO);
	gpioA.setAsAltFunc(2, Gpio::PA2_SPI0_CLK);

	Spi::config_t spi0Config = 
	{
		Spi::MODE_MAIN
	};

	spi0.enableClock();
	spi0.initialize(spi0Config);
	spi0.enableInterrupt();

	// LCD
	gpioA.setAsOutput(3); // CS
	gpioA.setAsOutput(4); // BL
	gpioA.setAsOutput(6); // DC
	
	gpioA.setOutput(4, true);

	Touch_LCD_Shield_for_Arduino_2_8_inch::config_t lcdConfig =
	{
		spi0,			//Spi &peri;
		{&gpioA, 3},	//pin_t chipSelect;
		{&gpioA, 6},	//pin_t dataCommand;
		{0, 0}			//pin_t reset;
	};

	brush.malloc(5000);

	lcd.initialize(lcdConfig);
	lcd.setFrameBuffer(brush);
	lcd.setBackgroundColor({0xFF, 0xFF, 0xFF});
	lcd.clear();
}

