/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG and 2.8-inch TFT Touch Shield.
 * @details Declares board initialization and global driver instances for the Waveshare 2.8-inch
 *          TFT Touch Shield Rev2.1 (ST7789V LCD, XPT2046 resistive touch controller, RGB565 frame
 *          buffer brush, GUI page task manager, and touch event dispatcher).
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef BSP__H_
#define BSP__H_

#include <stdint.h>
#include <2.8inch_TFT_Touch_Shield_Rev2.1/Touch_LCD_Shield_for_Arduino_2_8_inch.h>
#include <XPT2046/XPT2046.h>
#include <util/TaskManager.h>
#include <util/TouchEventDispatcher.h>

/**
 * @brief Initializes all on-board hardware peripherals for the 2.8-inch TFT Touch Shield.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Initializes SPI0, hardware CRC32, display controller, and resistive touch driver.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post SPI0, LCD display, touch controller, and event dispatcher are ready for operation.
 * @deprecated none
 */
void initializeBoard(void);

/**
 * @brief Global display driver instance for the 2.8-inch TFT Touch Shield (ST7789V over SPI0).
 */
extern Touch_LCD_Shield_for_Arduino_2_8_inch lcd;

/**
 * @brief Global RGB565 little-endian graphic frame buffer used for partial rendering operations.
 */
extern FrameBufferRgb565LE brush;

/**
 * @brief Task manager instance for handling GUI page task registration and page transitions.
 */
extern TaskManager page;

/**
 * @brief Resistive touch controller driver instance (XPT2046 over SPI0).
 */
extern XPT2046 touch;

/**
 * @brief Touch event dispatcher for routing touch press, drag, and release events to registered hit areas.
 */
extern TouchEventDispatcher dispatcher;

#endif


