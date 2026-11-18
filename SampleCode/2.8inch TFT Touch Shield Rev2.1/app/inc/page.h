/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page.h
 * @brief GUI page task declarations and identifiers for 2.8-inch TFT Touch Shield demo.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef PAGE__H_
#define PAGE__H_

#include <stdint.h>

/**
 * @brief Task ID for the logo display page.
 */
extern int16_t pageDisplayLogoId;

/**
 * @brief Task routine displaying board logo and system startup information.
 * @param[in] param Generic task parameter (unused, pass nullptr).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Displays Nuvoton logo, board name, and blink animation.
 * @pre LCD display must be initialized.
 * @post Page graphics are rendered and blinking loop runs until task switch.
 * @deprecated none
 */
void pageDisplayLogo(void* param);

/**
 * @brief Task ID for the main menu page.
 */
extern int16_t pageDisplayMainId;

/**
 * @brief Task routine for the main application menu page.
 * @param[in] param Generic task parameter (unused, pass nullptr).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Shows launcher icon for the drawing application.
 * @pre LCD display and touch dispatcher must be initialized.
 * @post Button event handlers are registered to dispatcher.
 * @deprecated none
 */
void pageDisplayMain(void* param);

/**
 * @brief Task ID for the touch calibration page.
 */
extern int16_t pageDisplayTouchCalibrationId;

/**
 * @brief Task routine for 2-point resistive touch calibration.
 * @param[in] param Generic task parameter (unused, pass nullptr).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning User must press target points until the progress bar completes.
 * @note Calibrates top-left and bottom-right touch coordinates, then saves to flash memory.
 * @pre LCD and XPT2046 touch driver must be initialized.
 * @post Calibration matrix is persisted to flash and active driver is updated.
 * @deprecated none
 */
void pageDisplayTouchCalibration(void* param);

/**
 * @brief Task ID for the free-hand drawing application page.
 */
extern int16_t pageDisplayDrawingId;

/**
 * @brief Task routine for the interactive drawing canvas.
 * @param[in] param Generic task parameter (unused, pass nullptr).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Offers 14 palette colors, low-pass filtered brush strokes, and an exit button.
 * @pre LCD, touch controller, and dispatcher must be operational.
 * @post Interactive canvas and palette dispatcher entries are active.
 * @deprecated none
 */
void pageDisplayDrawing(void* param);

#endif


