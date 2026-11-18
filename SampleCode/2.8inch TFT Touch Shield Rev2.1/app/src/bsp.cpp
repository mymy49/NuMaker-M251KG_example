/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG and 2.8-inch TFT Touch Shield.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

Touch_LCD_Shield_for_Arduino_2_8_inch lcd;

FrameBufferRgb565LE brush;

TaskManager page(8);

XPT2046 touch;

TouchEventDispatcher dispatcher(20, 2048);

/**
 * @details Configures peripheral pins and initializes hardware devices:
 *          1. SPI0: PA0 (MOSI), PA1 (MISO), PA2 (CLK) initialized in Master mode.
 *          2. CRC: Hardware CRC32 engine initialized with 0xFFFFFFFF polynomial seed.
 *          3. LCD: PA3 (CS), PA4 (Backlight), PA6 (DC) configured; frame buffer allocated and display cleared.
 *          4. Touch: PC3 (CS), PC5 (IRQ) configured for XPT2046 controller and linked to touch event dispatcher.
 * @see lcd, brush, touch, dispatcher, spi0, crc, initializeYss
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure SPI0 Peripheral (PA0: MOSI, PA1: MISO, PA2: CLK)
	// --------------------------------------------------
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

	// --------------------------------------------------
	// 2. Configure Hardware CRC Engine (CRC32 mode)
	// --------------------------------------------------
	Crc::config_t crcConfig = 
	{
		Crc::CRC_MODE_CRC32,	// mode_t mode;
		Crc::CRC_DAT_LEN_8BIT,	// datalen_t datalen;
		0xFFFFFFFF,				// uint32_t seed;
		false,					// bool writeBitOrderReverse;
		false					// bool checksumBitOrderReverse;
	};

	crc.enableClock();
	crc.initialize(crcConfig);
	crc.enableInterrupt();

	// --------------------------------------------------
	// 3. Configure 2.8-inch TFT LCD (ST7789V controller)
	// --------------------------------------------------
	gpioA.setAsOutput(3); // Chip Select (CS) pin
	gpioA.setAsOutput(4); // Backlight (BL) pin
	gpioA.setAsOutput(6); // Data/Command (DC) pin
	
	// Turn on LCD backlight
	gpioA.setOutput(4, true);

	Touch_LCD_Shield_for_Arduino_2_8_inch::config_t lcdConfig =
	{
		spi0,			// Spi &peri;
		{&gpioA, 3},	// pin_t chipSelect;
		{&gpioA, 6},	// pin_t dataCommand;
		{0, 0}			// pin_t reset;
	};

	// Allocate rendering brush buffer (5000 bytes)
	brush.malloc(5000);

	// Initialize LCD controller and bind frame buffer
	lcd.initialize(lcdConfig);
	lcd.setFrameBuffer(brush);
	lcd.setBackgroundColor({0xFF, 0xFF, 0xFF});
	lcd.clear();

	// --------------------------------------------------
	// 4. Configure Resistive Touch Controller (XPT2046)
	// --------------------------------------------------
	gpioC.setAsOutput(3); // Chip Select (CS) pin
	gpioC.setAsOutput(5); // Interrupt Request (IRQ) pin

	XPT2046::config_t touchConfig = 
	{
		spi0,			// Spi &spi;
		{&gpioC, 3},	// pin_t chipSelect;
		{0, 0},			// pin_t busy;
		{&gpioC, 5},	// pin_t isr;
	};

	touch.initialize(touchConfig);

	// Register touch device to the event dispatcher
	dispatcher.setPointerDevice(touch);
}


