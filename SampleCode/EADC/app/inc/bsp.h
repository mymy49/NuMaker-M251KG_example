/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG EADC example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>

/**
 * @brief Initializes on-board peripherals and EADC channels.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Configures PB0 (CH0) and PB1 (CH1) as analog inputs and starts conversions.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post EADC channels 0 and 1 are sampling continuously in background.
 * @deprecated none
 */
void initializeBoard(void);

#endif


