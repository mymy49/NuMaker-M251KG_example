/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG CRC example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>

/**
 * @brief Initializes on-board peripherals and hardware CRC engine.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Enables CRC peripheral clock and sets up CRC32 mode.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post Hardware CRC engine is initialized and ready for calculation.
 * @deprecated none
 */
void initializeBoard(void);

#endif


