/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG UART example.
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

#endif


