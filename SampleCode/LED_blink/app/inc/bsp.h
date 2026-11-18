/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG LED blink example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>
#include <dev/led.h>

/**
 * @brief Initializes on-board peripherals and user LED GPIO.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Configures PB14 as output mode for the red LED.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post The on-board red LED is initialized and set to the OFF state.
 * @deprecated none
 */
void initializeBoard(void);

#endif


