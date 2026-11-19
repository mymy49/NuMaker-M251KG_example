/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file led.h
 * @brief Header file for on-board LED control functions on the NuMaker-M251KG evaluation board.
 * @details Declares initialization and state control functions for the on-board user indicator LED
 *          connected to GPIO Port B Pin 14 (PB14).
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef DEV_LED__H_
#define DEV_LED__H_

/**
 * @namespace Led
 * @brief Provides control routines for the on-board user LED.
 * @details Encapsulates hardware-specific GPIO configuration and active-low output control
 *          for the red status indicator LED on the NuMaker-M251KG development board.
 *
 * Example usage:
 * @code
 * #include <yss.h>
 * #include <dev/led.h>
 *
 * int main(void)
 * {
 *     initializeYss();
 *     Led::initialize();
 *
 *     while (1)
 *     {
 *         Led::red(true);
 *         thread::delay(500);
 *         Led::red(false);
 *         thread::delay(500);
 *     }
 * }
 * @endcode
 */
namespace Led 
{
	/**
	 * @brief Initializes the GPIO pin for the on-board red LED.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Configures PB14 as an output pin and turns off the LED.
	 * @pre GPIO peripheral clocks must be properly configured via initializeYss().
	 * @post The PB14 pin is configured as push-pull output and set to the OFF state (HIGH).
	 * @deprecated none
	 */
	void initialize(void);	

	/**
	 * @brief Sets the on-board red LED output state.
	 * @param[in] on Boolean flag to set LED state (true: turn ON, false: turn OFF).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note The on-board LED is wired in an active-low circuit (LOW = ON, HIGH = OFF).
	 * @pre Led::initialize() must be called prior to invoking this function.
	 * @post The logic level of PB14 is updated according to the on parameter.
	 * @deprecated none
	 */
	void red(bool on);
}

#endif


