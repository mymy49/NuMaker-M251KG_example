/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG UART example.
 * @details Declares board initialization and asynchronous LED trigger routines for the UART0 echo application.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>
#include <dev/led.h>

/**
 * @brief Initializes on-board hardware peripherals, LED, and UART0 interface.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Configures PB12 (RXD) and PB13 (TXD) for UART0 at 115200 baud, 8N1.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post UART0 and user LED are initialized and ready for communication.
 * @deprecated none
 */
void initializeBoard(void);

/**
 * @brief Asynchronous RTOS trigger handler that briefly flashes the on-board red LED.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Invoked asynchronously by the yss OS trigger manager upon UART reception events.
 * @note Illuminates the red LED for 20 milliseconds and then turns it off.
 * @pre `Led::initialize()` must have been called and this function registered via `trigger::add()`.
 * @post The on-board red LED pulses ON for 20 ms and returns to the OFF state.
 * @deprecated none
 */
void trigger_blinkLed(void);

#endif


