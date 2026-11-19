/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG EADC example.
 * @details Declares board-level initialization routines for configuring GPIO analog input pins
 *          and starting continuous multi-channel Enhanced Analog-to-Digital Converter (EADC) sampling.
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
 * @warning Ensure input voltage on PB0 and PB1 does not exceed the analog reference voltage (AVDD).
 * @note Configures PB0 (EADC0_CH0) and PB1 (EADC0_CH1) as analog inputs with 14-bit resolution and LPF.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post EADC channels 0 and 1 are sampling continuously in the background via interrupt-driven updates.
 * @deprecated none
 */
void initializeBoard(void);

#endif


