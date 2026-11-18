/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file dev_led.cpp
 * @brief Implementation of on-board LED control functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <dev/led.h>
#include <yss/instance.h>

namespace Led 
{
	/**
	 * @details Configures GPIO port B pin 14 as output mode to control the on-board red LED.
	 *          Initializes the LED to an inactive (turned off) state by calling red(false).
	 * @see Led::red, gpioB
	 * @todo none
	 * @bug none
	 */
	void initialize(void)
	{
		// Configure PB14 as a general purpose push-pull output pin
		gpioB.setAsOutput(14);

		// Initialize the LED to the OFF state
		red(false);
	}

	/**
	 * @details Controls the logic level of GPIO port B pin 14.
	 *          Since the on-board red LED circuit is configured as active-low,
	 *          driving the pin LOW illuminates the LED, and HIGH turns it off.
	 * @see Led::initialize, gpioB
	 * @todo none
	 * @bug none
	 */
	void red(bool on)
	{
		// Output LOW for active-on, HIGH for active-off
		gpioB.setOutput(14, !on);
	}
}


