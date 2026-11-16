/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file XPT2046.h
 * @brief Driver for the XPT2046 4-wire resistive touch screen controller.
 */

#ifndef	YSS_MODULE_XPT2046__H_
#define	YSS_MODULE_XPT2046__H_

#include <hal/ResistiveTouch.h>
#include <drv/Spi.h>
#include <drv/Gpio.h>
#include <yss/Trigger.h>

/**
 * @class XPT2046
 * @brief Driver class for the XPT2046 4-wire resistive touch screen controller.
 *
 * @details
 * The XPT2046 is a 4-wire resistive touch screen controller featuring a 12-bit 125 kHz
 * SAR Analog-to-Digital Converter (ADC). It interfaces with the microcontroller via SPI
 * and asserts an active-low interrupt signal (PENIRQ) whenever touch contact is detected.
 *
 * ### Inherited Classes & Module Origins
 * - `ResistiveTouch` (from `hal/ResistiveTouch.h` in the yss OS HAL layer):
 *   Provides calibrated coordinate conversions (`calculateX()`, `calculateY()`) and pointer
 *   event dispatching (`pushDownEvent()`, `pushDragEvent()`, `pushUpEvent()`).
 * - `Trigger` (from `yss/Trigger.h` in the yss OS core system):
 *   Enables asynchronous execution in an independent OS thread.
 *   The virtual function `trigger()` is overridden to handle debouncing, continuous coordinate
 *   sampling while pressed, drag notification, and release detection.
 *
 * ### Example Usage
 * @code
 * #include <module/XPT2046/XPT2046.h>
 *
 * XPT2046 touch;
 * TouchEventDispatcher dispatcher(20, 2048);
 *
 * void initializeTouch(void)
 * {
 *     // Configure GPIO pins and SPI peripheral beforehand.
 *     XPT2046::config_t touchConfig = 
 *     {
 *         spi0,           // Spi &spi;
 *         {&gpioC, 3},    // pin_t chipSelect;
 *         {0, 0},         // pin_t busy; (optional / not connected)
 *         {&gpioC, 5},    // pin_t isr; (PENIRQ pin)
 *     };
 *
 *     touch.initialize(touchConfig);
 *     dispatcher.setPointerDevice(touch);
 * }
 * @endcode
 */
class XPT2046 : public ResistiveTouch, protected Trigger
{
public :
	/**
	 * @struct config_t
	 * @brief Hardware configuration parameters for initializing the XPT2046 driver.
	 */
	struct config_t
	{
		Spi &spi;           ///< Reference to the SPI peripheral instance connected to the touch controller.
		pin_t chipSelect;   ///< GPIO pin mapped to the active-low Chip Select (CS) line.
		pin_t busy;         ///< GPIO pin mapped to the BUSY status line (set {0, 0} if unused).
		pin_t isr;          ///< GPIO pin mapped to the active-low touch interrupt (PENIRQ) line.
	};

	/**
	 * @brief Constructs a new XPT2046 driver object.
	 * @param None.
	 * @return None.
	 * @author Yoon-Ki Hong
	 * @date 2026
	 * @warning None.
	 * @note Initializes member pointers to null. Hardware configuration occurs in initialize().
	 * @pre None.
	 * @post Instance is constructed.
	 * @deprecated None.
	 */
	XPT2046();

	/**
	 * @brief Initializes the XPT2046 touch screen controller and sets up the touch interrupt.
	 * @param[in] config Hardware configuration containing SPI and GPIO pin assignments.
	 * @return `error_t::ERROR_NONE` upon successful initialization.
	 * @author Yoon-Ki Hong
	 * @date 2026
	 * @warning SPI and GPIO clocks must be enabled and pins configured before calling this function.
	 * @note Spawns a Trigger execution thread with a 512-byte stack and configures falling-edge interrupt on the PENIRQ pin.
	 * @pre SPI peripheral and GPIO ports must be enabled.
	 * @post Touch controller is powered up and ready to generate events.
	 * @deprecated None.
	 */
	error_t initialize(config_t config);
	
protected :
	/**
	 * @union XPT2046_Control_t
	 * @brief 8-bit control byte format for XPT2046 command transactions over SPI.
	 */
	union XPT2046_Control_t
	{
	    struct 
		{
	        uint8_t pd      : 2;    ///< Bit 0-1: Power-Down mode selection (00 = Power-down between conversions, PENIRQ enabled).
	        uint8_t ser_dfr : 1;    ///< Bit 2: Single-Ended (1) / Differential (0) reference mode.
	        uint8_t mode    : 1;    ///< Bit 3: ADC resolution (0 = 12-bit, 1 = 8-bit).
	        uint8_t channel : 3;    ///< Bit 4-6: Channel select bits A2-A0 (001 = Y-position, 101 = X-position).
	        uint8_t s       : 1;    ///< Bit 7: Start bit (must always be 1).
	    } bits;

	    uint8_t byte;               ///< Full 8-bit command byte sent over SPI.
	};

	const XPT2046_Control_t mConXpos = 
	{
		0,	//uint8_t pd      : 2;
		0,	//uint8_t ser_dfr : 1;
		0,	//uint8_t mode    : 1;
		5,	//uint8_t channel : 3;
		1	//uint8_t s       : 1;
	};

	const XPT2046_Control_t mConYpos = 
	{
		0,	//uint8_t pd      : 2;
		0,	//uint8_t ser_dfr : 1;
		0,	//uint8_t mode    : 1;
		1,	//uint8_t channel : 3;
		1	//uint8_t s       : 1;
	};

	/**
	 * @brief Transmits a control byte to the XPT2046 and reads the 12-bit ADC conversion result.
	 * @param[in] control 8-bit control structure selecting the channel, conversion mode, and power settings.
	 * @return 12-bit raw conversion result from the ADC.
	 * @author Yoon-Ki Hong
	 * @date 2026
	 * @warning Locks the SPI peripheral mutex during transmission.
	 * @note Waits 100 us between command transmission and data readout for conversion settling.
	 * @pre `initialize()` must be called beforehand.
	 * @post SPI peripheral is unlocked.
	 * @deprecated None.
	 */
	uint16_t convert(XPT2046_Control_t control);

private :
	Spi *mSpi;          ///< Pointer to the SPI peripheral instance.
	pin_t mCsPin;       ///< GPIO pin for active-low Chip Select.
	pin_t mBusyPin;     ///< GPIO pin for BUSY status indicator.
	pin_t mIsrPin;      ///< GPIO pin for active-low touch interrupt (PENIRQ).

	/**
	 * @brief Trigger thread execution function that processes touch interrupts and generates events.
	 * @param None.
	 * @return None.
	 * @author Yoon-Ki Hong
	 * @date 2026
	 * @warning Overrides `Trigger::trigger()`. Internal handler invoked by the thread scheduler.
	 * @note Runs in its own thread context upon falling-edge PENIRQ trigger. Continuously samples coordinates while touch is held.
	 * @pre Trigger thread must be activated via `activateTrigger()`.
	 * @post Emits pushDownEvent, pushDragEvent, and pushUpEvent to the registered pointer listener.
	 * @deprecated None.
	 */
	void trigger() override;
};

#endif
