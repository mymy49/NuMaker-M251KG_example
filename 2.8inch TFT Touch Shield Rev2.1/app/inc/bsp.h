/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>
#include <2.8inch_TFT_Touch_Shield_Rev2.1/Touch_LCD_Shield_for_Arduino_2_8_inch.h>
#include <util/DisplayPageManager.h>

void initializeBoard(void);

extern Touch_LCD_Shield_for_Arduino_2_8_inch lcd;

extern FrameBufferRgb565LE brush;

extern DisplayPageManager page;

#endif

