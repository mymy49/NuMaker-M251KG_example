/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file module_XPT2046.cpp
 * @brief Implementation of the XPT2046 resistive touch screen controller driver.
 */

#include "XPT2046.h"
#include <yss/debug.h>

/**
 * @brief Default SPI communication parameters for the XPT2046 controller.
 * @details Configured for SPI Mode 0, 2 MHz maximum clock frequency, and 8-bit data transfers.
 */
static const Spi::specification_t gSpiSpec =
{
	Spi::CLOCK_MODE_MODE0,	//uint8_t mode;
	2000000,				//uint32_t maxFreq;
	Spi::BIT_BIT8			//uint8_t bit;
};

/**
 * @details Default constructor. Initializes the internal SPI pointer to nullptr.
 * Hardware peripheral registration and thread activation are deferred to initialize().
 * @see XPT2046::initialize()
 * @todo None.
 * @bug None.
 */
XPT2046::XPT2046()
{
	// Initialize the SPI peripheral pointer to nullptr
	mSpi = nullptr;
}

/**
 * @details Stores the SPI peripheral pointer and GPIO pin assignments (Chip Select, Busy, ISR).
 * Activates the Trigger thread with a 512-byte stack size for asynchronous touch processing.
 * Configures the ISR GPIO pin (PENIRQ) for falling-edge detection and enables its interrupt.
 * Sets the Chip Select pin high (inactive), introduces a 100 ms stabilization delay,
 * and performs an initial conversion on the X-channel to power up the internal ADC and re-enable PENIRQ.
 * @see XPT2046::convert()
 * @see Gpio::setGpioInterrupt()
 * @todo None.
 * @bug None.
 */
error_t XPT2046::initialize(config_t config)
{
	// Store SPI peripheral instance and pin configurations
	mSpi = &config.spi;
	mCsPin = config.chipSelect;
	mBusyPin = config.busy;
	mIsrPin = config.isr;

	// Activate the Trigger execution thread with a 512-byte stack
	activateTrigger(512);

	// Configure the touch interrupt (PENIRQ) pin for falling-edge trigger
	mIsrPin.port->setGpioInterrupt(mIsrPin.pin, Gpio::EDGE_FALLING, getTriggerId());
	mIsrPin.port->enableInterrupt(true);
	
	// Deassert chip select (active low)
	mCsPin.port->setOutput(mCsPin.pin, true);

	// Wait 100 ms for hardware stabilization
	thread::delay(100);
	
	// Perform dummy conversion to initialize ADC and re-enable PENIRQ output
	convert(mConXpos);
	
	return error_t::ERROR_NONE;
}

/**
 * @details Locks the SPI bus mutex and applies the 2 MHz Mode 0 SPI specification.
 * Asserts the Chip Select line (low) and transmits the 8-bit control byte specifying channel,
 * 12-bit resolution, and power mode. Pauses for 100 us to allow the internal SAR ADC conversion
 * to settle. Clocks out the 12-bit result across two byte transfers (MSB-aligned in bits [11:5]
 * of the first byte and bits [4:0] of the second byte). Deasserts Chip Select (high) and unlocks the SPI bus.
 * @see XPT2046_Control_t
 * @todo None.
 * @bug None.
 */
uint16_t XPT2046::convert(XPT2046_Control_t control)
{
	uint16_t data;

	// Acquire SPI bus lock for exclusive access
	mSpi->lock();

	// Set SPI communication specification (Mode 0, 2 MHz, 8-bit)
	mSpi->setSpecification(gSpiSpec);

	// Assert Chip Select (active low) to start communication
	mCsPin.port->setOutput(mCsPin.pin, false);

	// Transmit control command byte to select channel, resolution, and power mode
	mSpi->exchange(control.byte);

	// Wait 100 us for internal sampling capacitor acquisition and conversion
	thread::delayUs(100);

	// Read upper conversion bits (shift left by 5 to place into bits [11:5])
	data = mSpi->exchange(0) << 5;

	// Read lower conversion bits (shift right by 3 to place into bits [4:0])
	data |= mSpi->exchange(0) >> 3;

	// Deassert Chip Select (inactive high)
	mCsPin.port->setOutput(mCsPin.pin, true);

	// Release SPI bus lock
	mSpi->unlock();

	// Return combined 12-bit ADC result
	return data;
}

/**
 * @details Overrides `Trigger::trigger()` to handle touch interrupt events in an independent thread.
 * Introduces an initial 30 ms delay to debounce physical contact bounce. Samples the X and Y
 * coordinates via `convert()`, converts them to screen coordinates via `calculateX()` and `calculateY()`,
 * and emits a touch press event via `pushDownEvent()`.
 * While the PENIRQ pin remains low (indicating continuous touch contact), repeatedly samples coordinates,
 * emits `pushDragEvent()`, and calls `thread::yield()` to prevent starvation of lower-priority threads.
 * Once the touch pin returns high (touch released), emits a final `pushUpEvent()`.
 * @see ResistiveTouch::calculateX()
 * @see ResistiveTouch::calculateY()
 * @see ResistiveTouch::pushDownEvent()
 * @see ResistiveTouch::pushDragEvent()
 * @see ResistiveTouch::pushUpEvent()
 * @todo None.
 * @bug None.
 */
void XPT2046::trigger()
{
	// Debounce delay to stabilize touch contact
	thread::delay(30);

	// Sample initial touch coordinates and calibrate
	uint16_t x = calculateX(convert(mConXpos));
	uint16_t y = calculateY(convert(mConYpos));

	// Emit touch press (down) event
	pushDownEvent(x, y);

	// Continue sampling while the screen remains touched (PENIRQ active low)
	while(!mIsrPin.port->getInputData(mIsrPin.pin))
	{
		// Sample updated coordinates during continuous touch
		x = calculateX(convert(mConXpos));
		y = calculateY(convert(mConYpos));
		
		// Emit touch drag event
		pushDragEvent(x, y);

		// Yield thread to allow other tasks to execute
		thread::yield();
	}

	// Emit touch release (up) event
	pushUpEvent(x, y);
}
