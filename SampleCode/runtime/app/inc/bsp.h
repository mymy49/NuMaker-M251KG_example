/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG runtime timer example.
 * @details Declares board-level initialization routines for the high-resolution runtime measurement example.
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
 * @note Hardware timers required for runtime tracking (`YSS_RUNTIME_TIMER`) are initialized automatically by `initializeYss()`.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post Hardware board configuration is complete.
 * @deprecated none
 */
void initializeBoard(void);

#endif


