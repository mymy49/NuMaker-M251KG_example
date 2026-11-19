/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file page.h
 * @brief GUI page task declarations, touch callbacks, and identifiers for 2.8-inch TFT Touch Shield demo.
 * @details Declares task IDs, entry functions, palette helpers, and touch event callbacks for the
 *          logo splash screen, main menu launcher, 2-point resistive touch calibration screen,
 *          and interactive free-hand drawing canvas.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef PAGE__H_
#define PAGE__H_

#include <stdint.h>
#include <bsp.h>

/**
 * @brief Task ID for the logo display page registered in `page` TaskManager.
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
 * @brief Task ID for the main menu page registered in `page` TaskManager.
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
 * @brief Touch callback triggered when the Drawing application icon on the main menu is pressed.
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Transitions the active task in `page` TaskManager to `pageDisplayDrawingId` on `EVENT_TOUCH_DOWN`.
 * @pre `pageDisplayDrawingId` must be registered in `page` TaskManager.
 * @post The drawing application page is launched.
 * @deprecated none
 */
void callback_drawing(PointerDevice::event_t event);

/**
 * @brief Task ID for the touch calibration page registered in `page` TaskManager.
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
 * @brief Touch event callback used during the 2-point resistive touch calibration sequence.
 * @param[in] event Pointer device touch event containing touch state (`EVENT_TOUCH_DOWN`, `EVENT_TOUCH_DRAG`, `EVENT_TOUCH_UP`) and coordinates.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Feeds raw uncalibrated ADC values from the XPT2046 driver into moving-average low-pass filters.
 * @note Registered across the full 240x320 screen area while `pageDisplayTouchCalibration` is active.
 * @pre `touch` driver must be initialized and active.
 * @post Calibration flags and filtered X/Y ADC readings (`gLpfX`, `gLpfY`) are updated.
 * @deprecated none
 */
void callback_calibrate(PointerDevice::event_t event);

/**
 * @brief Task ID for the free-hand drawing application page registered in `page` TaskManager.
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

/**
 * @brief Draws a top-row palette swatch button on the LCD.
 * @param[in] area Screen area bounding box for the palette button.
 * @param[in] bdColor Outer border color indicating normal or selected state.
 * @param[in] color Inner swatch fill color.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Caller must hold `lcd.lock()` or ensure single-threaded LCD access.
 * @note Renders a 26x14 inner color rectangle offset by (2, 4) within a 30x20 button area.
 * @pre `brush` and `lcd` must be initialized.
 * @post The specified top-row palette swatch is drawn on the display.
 * @deprecated none
 */
void drawPaletteTop(Area area, Color bdColor, Color color);

/**
 * @brief Draws a bottom-row palette swatch button on the LCD.
 * @param[in] area Screen area bounding box for the palette button.
 * @param[in] bdColor Outer border color indicating normal or selected state.
 * @param[in] color Inner swatch fill color.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Caller must hold `lcd.lock()` or ensure single-threaded LCD access.
 * @note Renders a 26x14 inner color rectangle offset by (2, 2) within a 30x20 button area.
 * @pre `brush` and `lcd` must be initialized.
 * @post The specified bottom-row palette swatch is drawn on the display.
 * @deprecated none
 */
void drawPaletteBot(Area area, Color bdColor, Color color);

/**
 * @brief Draws a palette swatch button by index (0 to 13) with the given border and fill colors.
 * @param[in] index Palette index (0..6 for top row, 7..13 for bottom row).
 * @param[in] bdColor Outer border color indicating normal or selected state.
 * @param[in] color Inner swatch fill color.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Indices outside 0..13 are ignored.
 * @note Delegates rendering to `drawPaletteTop()` or `drawPaletteBot()`.
 * @pre `initializePalette()` must have been called to populate `gPaletteArea`.
 * @post The target palette button on the LCD is redrawn.
 * @deprecated none
 */
void drawPalette(uint32_t index, Color bdColor, Color color);

/**
 * @brief Initializes the 14 color palette button areas and draws them at the bottom of the screen.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Must be called while holding `lcd.lock()` during drawing page initialization.
 * @note Creates two rows of seven 30x20 color buttons at the bottom-left of the 240x320 screen.
 * @pre `lcd` and `brush` must be initialized.
 * @post `gPaletteArea[0..13]` are populated and rendered on the display.
 * @deprecated none
 */
void initializePalette();

/**
 * @brief Touch callback selecting palette index 0 (Black: `#000000`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 0 and button borders are updated.
 * @deprecated none
 */
void callback_Palette0(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 1 (White: `#FFFFFF`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 1 and button borders are updated.
 * @deprecated none
 */
void callback_Palette1(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 2 (Red: `#FF0000`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 2 and button borders are updated.
 * @deprecated none
 */
void callback_Palette2(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 3 (Green: `#00FF00`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 3 and button borders are updated.
 * @deprecated none
 */
void callback_Palette3(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 4 (Blue: `#0000FF`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 4 and button borders are updated.
 * @deprecated none
 */
void callback_Palette4(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 5 (Yellow: `#FFFF00`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 5 and button borders are updated.
 * @deprecated none
 */
void callback_Palette5(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 6 (Magenta: `#FF00FF`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 6 and button borders are updated.
 * @deprecated none
 */
void callback_Palette6(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 7 (Cyan: `#00FFFF`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 7 and button borders are updated.
 * @deprecated none
 */
void callback_Palette7(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 8 (Orange: `#FF5500`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 8 and button borders are updated.
 * @deprecated none
 */
void callback_Palette8(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 9 (Purple: `#800080`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 9 and button borders are updated.
 * @deprecated none
 */
void callback_Palette9(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 10 (Pink: `#FFC0CB`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 10 and button borders are updated.
 * @deprecated none
 */
void callback_Palette10(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 11 (Gold: `#FFD700`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 11 and button borders are updated.
 * @deprecated none
 */
void callback_Palette11(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 12 (Silver: `#C0C0C0`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 12 and button borders are updated.
 * @deprecated none
 */
void callback_Palette12(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 13 (Navy: `#000080`).
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Updates active palette selection on `EVENT_TOUCH_DOWN`.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 13 and button borders are updated.
 * @deprecated none
 */
void callback_Palette13(PointerDevice::event_t event);

/**
 * @brief Touch callback selecting palette index 14 if defined.
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Index 14 is outside the default 14-color palette array (`0..13`).
 * @note Reserved callback for an optional 15th palette slot.
 * @pre `initializePalette()` must be called first.
 * @post `gLastSelectedPalette` is set to 14.
 * @deprecated none
 */
void callback_Palette14(PointerDevice::event_t event);

/**
 * @brief Touch callback handling free-hand drawing strokes on the canvas area.
 * @param[in] event Touch event containing current touch state and screen coordinates.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Acquires `lcd.lock()` while drawing line segments onto the display.
 * @note Applies a low-pass filter (`Lpf`) to X and Y drag coordinates to smooth resistive touch jitter.
 * @pre `lcd` must be initialized and `callback_draw` registered to the canvas area in `dispatcher`.
 * @post A smoothed line segment in the active palette color is rendered on the LCD canvas.
 * @deprecated none
 */
void callback_draw(PointerDevice::event_t event);

/**
 * @brief Touch callback for the Exit ("X") button on the drawing page.
 * @param[in] event Touch event structure from `TouchEventDispatcher`.
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Switches the active task back to `pageDisplayMainId` on `EVENT_TOUCH_DOWN`.
 * @pre `pageDisplayMainId` must be registered in `page` TaskManager.
 * @post Active GUI page transitions from the drawing canvas back to the main menu.
 * @deprecated none
 */
void callback_exit(PointerDevice::event_t event);

#endif


