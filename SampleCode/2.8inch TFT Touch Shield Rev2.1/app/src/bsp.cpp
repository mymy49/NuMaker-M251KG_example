/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG and 2.8-inch TFT Touch Shield.
 * @details Instantiates and configures the SPI0 bus, hardware CRC32 peripheral, ST7789V 2.8-inch
 *          TFT LCD driver, RGB565 frame buffer, XPT2046 resistive touch controller, GUI page
 *          task manager, and touch event dispatcher.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @brief Global display driver instance for the Waveshare 2.8-inch TFT Touch Shield Rev2.1.
 */
Touch_LCD_Shield_for_Arduino_2_8_inch lcd;

/**
 * @brief Global RGB565 little-endian frame buffer used as a scratchpad brush for text and UI rendering.
 */
FrameBufferRgb565LE brush;

/**
 * @brief Global GUI task manager configured to manage up to 8 page tasks.
 */
TaskManager page(8);

/**
 * @brief Global XPT2046 resistive touch screen controller instance.
 */
XPT2046 touch;

/**
 * @brief Global touch event dispatcher configured for up to 20 touch regions and a 2048-byte worker stack.
 */
TouchEventDispatcher dispatcher(20, 2048);

/**
 * @details Configures peripheral pins and initializes hardware devices:
 *          1. SPI0: PA0 (MOSI), PA1 (MISO), PA2 (CLK) initialized in Master mode (`Spi::MODE_MAIN`).
 *          2. CRC: Hardware CRC32 engine initialized in 8-bit data width mode with `0xFFFFFFFF` polynomial seed.
 *          3. LCD: PA3 (CS), PA4 (Backlight), PA6 (DC) configured; 5000-byte frame buffer allocated and display cleared to white.
 *          4. Touch: PC3 (CS), PC5 (IRQ) configured for XPT2046 controller and linked to the touch event dispatcher.
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
		Spi::MODE_MAIN			// Configure SPI0 in master (main) operating mode
	};

	// Enable SPI0 peripheral clock, apply master configuration, and enable SPI0 interrupts
	spi0.enableClock();
	spi0.initialize(spi0Config);
	spi0.enableInterrupt();

	// --------------------------------------------------
	// 2. Configure Hardware CRC Engine (CRC32 mode)
	// --------------------------------------------------
	Crc::config_t crcConfig = 
	{
		Crc::CRC_MODE_CRC32,	// mode_t mode: 32-bit CRC-32 polynomial
		Crc::CRC_DAT_LEN_8BIT,	// datalen_t datalen: 8-bit byte-wise CRC calculation
		0xFFFFFFFF,				// uint32_t seed: Standard 0xFFFFFFFF initial seed
		false,					// bool writeBitOrderReverse: Normal write bit order
		false					// bool checksumBitOrderReverse: Normal output checksum bit order
	};

	// Enable CRC peripheral clock, initialize CRC registers, and enable interrupts
	crc.enableClock();
	crc.initialize(crcConfig);
	crc.enableInterrupt();

	// --------------------------------------------------
	// 3. Configure 2.8-inch TFT LCD (ST7789V controller)
	// --------------------------------------------------
	gpioA.setAsOutput(3); // LCD Chip Select (CS) pin on PA3
	gpioA.setAsOutput(4); // LCD Backlight (BL) control pin on PA4
	gpioA.setAsOutput(6); // LCD Data/Command (DC) selection pin on PA6
	
	// Drive PA4 HIGH to turn on the LCD backlight
	gpioA.setOutput(4, true);

	Touch_LCD_Shield_for_Arduino_2_8_inch::config_t lcdConfig =
	{
		spi0,			// Spi &peri: Shared SPI0 bus instance
		{&gpioA, 3},	// pin_t chipSelect: PA3
		{&gpioA, 6},	// pin_t dataCommand: PA6
		{0, 0}			// pin_t reset: Connected to hardware reset (not software controlled)
	};

	// Allocate 5000-byte rendering scratchpad buffer on the heap
	brush.malloc(5000);

	// Initialize ST7789V LCD controller, bind scratchpad frame buffer, and clear screen to white
	lcd.initialize(lcdConfig);
	lcd.setFrameBuffer(brush);
	lcd.setBackgroundColor({0xFF, 0xFF, 0xFF});
	lcd.clear();

	// --------------------------------------------------
	// 4. Configure Resistive Touch Controller (XPT2046)
	// --------------------------------------------------
	gpioC.setAsOutput(3); // Touch Chip Select (CS) pin on PC3
	gpioC.setAsOutput(5); // Touch Interrupt Request (PENIRQ) pin on PC5

	XPT2046::config_t touchConfig = 
	{
		spi0,			// Spi &spi: Shared SPI0 bus instance
		{&gpioC, 3},	// pin_t chipSelect: PC3
		{0, 0},			// pin_t busy: Unused
		{&gpioC, 5},	// pin_t isr: PC5 (PENIRQ)
	};

	// Initialize XPT2046 driver and bind it as the pointer device for the touch event dispatcher
	touch.initialize(touchConfig);
	dispatcher.setPointerDevice(touch);
}


