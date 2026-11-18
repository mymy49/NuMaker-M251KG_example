/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file led.h
 * @brief Header file for on-board LED control functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef DEV_LED__H_
#define DEV_LED__H_

/**
 * @namespace Led
 * @brief Provides control routines for the on-board user LED.
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
	 * @pre GPIO peripheral clocks must be properly configured.
	 * @post The PB14 pin is configured as push-pull output and set to OFF.
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
	 * @note The on-board LED is wired in an active-low circuit.
	 * @pre Led::initialize() must be called prior to invoking this function.
	 * @post The state of PB14 is updated according to the on parameter.
	 * @deprecated none
	 */
	void red(bool on);
}

#endif


