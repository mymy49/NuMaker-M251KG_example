/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file dev_led.cpp
 * @brief Implementation of on-board LED control functions.
 * @details Provides hardware-level GPIO manipulation for the NuMaker-M251KG user LED on PB14
 *          using the yss OS Gpio driver instance (`gpioB`).
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <dev/led.h>
#include <yss/instance.h>

namespace Led 
{
	/**
	 * @details Configures GPIO port B pin 14 as push-pull output mode to drive the on-board red LED.
	 *          Immediately initializes the LED to an inactive (turned off) state by calling red(false)
	 *          so the LED does not glitch on startup.
	 * @see Led::red, gpioB, Gpio::setAsOutput
	 * @todo none
	 * @bug none
	 */
	void initialize(void)
	{
		// Configure PB14 as a general-purpose push-pull output pin via the yss OS GPIO driver
		gpioB.setAsOutput(14);

		// Drive PB14 HIGH to initialize the active-low LED to the OFF state
		red(false);
	}

	/**
	 * @details Controls the logic level of GPIO port B pin 14.
	 *          Since the on-board red LED circuit is wired in an active-low configuration
	 *          (anode tied to VCC through a current-limiting resistor, cathode connected to PB14),
	 *          driving the pin LOW illuminates the LED, and driving it HIGH turns it off.
	 * @see Led::initialize, gpioB, Gpio::setOutput
	 * @todo none
	 * @bug none
	 */
	void red(bool on)
	{
		// Invert the boolean flag so true outputs logic LOW (LED ON) and false outputs logic HIGH (LED OFF)
		gpioB.setOutput(14, !on);
	}
}


