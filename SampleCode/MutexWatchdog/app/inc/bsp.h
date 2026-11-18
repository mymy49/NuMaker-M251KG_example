/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG Mutex Watchdog example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>

/**
 * @brief Initializes on-board hardware peripherals.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Configures PB14 as output for LED status indication.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post PB14 pin is configured as push-pull output.
 * @deprecated none
 */
void initializeBoard(void);

/**
 * @brief Sets the state of the on-board red LED indicator.
 * @param[in] on Boolean flag to set LED state (true: turn ON, false: turn OFF).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Active-low circuit logic: driving LOW turns the LED on.
 * @pre initializeBoard() must have been called.
 * @post The logic level on PB14 is updated.
 * @deprecated none
 */
void setLed(bool on);

#endif


