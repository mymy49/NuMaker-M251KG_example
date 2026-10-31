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

TaskManager page(8);

XPT2046 touch;

TouchEventDispatcher dispatcher(10, 2048);

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

	// CRC32
	Crc::config_t crcConfig = 
	{
		Crc::CRC_MODE_CRC32,	//mode_t mode;
		Crc::CRC_DAT_LEN_8BIT,	//datalen_t datalen;
		0xFFFFFFFF,				//uint32_t seed;
		false,					//bool writeBitOrderReverse;
		false					//bool checksumBitOrderReverse;
	};

	crc.enableClock();
	crc.initialize(crcConfig);
	crc.enableInterrupt();

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

	// Touch
	gpioC.setAsOutput(3); // CS
	gpioC.setAsOutput(5); // IRQ

	XPT2046::config_t touchConfig = 
	{
		spi0,			//Spi &spi;
		{&gpioC, 3},	//pin_t chipSelect;
		{0, 0},			//pin_t busy;
		{&gpioC, 5},	//pin_t isr;
	};

	touch.initialize(touchConfig);

	dispatcher.setPointerDevice(touch);
}

