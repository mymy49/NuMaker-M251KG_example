/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.h
 * @brief Board Support Package header for NuMaker-M251KG Mutex Watchdog example.
 * @details Declares board initialization, LED control, and RTOS mutex watchdog demonstration routines.
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
 * @note Configures PB14 as output for LED status indication.
 * @pre yss OS core and system clock must be initialized via initializeYss().
 * @post PB14 pin is configured as push-pull output.
 * @deprecated none
 */
void initializeBoard(void);

/**
 * @brief Sets the state of the on-board red LED indicator.
 * @param[in] on Boolean flag to set LED state (true: turn ON, false: turn OFF).
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Active-low circuit logic: driving LOW turns the LED on.
 * @pre initializeBoard() must have been called.
 * @post The logic level on PB14 is updated.
 * @deprecated none
 */
void setLed(bool on);

/**
 * @brief Thread entry point designed to induce a mutex deadlock / watchdog trigger.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Deliberately holds `gMutex` without releasing to trigger `mutexWatchdogHandler()`.
 * @note Waits 5 seconds before locking the shared mutex indefinitely.
 * @pre `gMutex` must be initialized and `THREAD_WATCHDOG_ENABLE` must be enabled in `config.h`.
 * @post `gMutex` is locked indefinitely by this thread, causing a watchdog timeout on waiting threads.
 * @deprecated none
 */
void thread_testMutexWatchdog(void);

/**
 * @brief Custom recovery handler invoked when a mutex lock exceeds `THREAD_WATCHDOG_OVERFLOW_TIME`.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Overrides the default yss OS weak handler (which performs a system reset) with an infinite alarm blink loop.
 * @note Toggles the on-board LED at 500 ms intervals to visually signal a detected mutex deadlock.
 * @pre `THREAD_WATCHDOG_ENABLE` must be set to `true` in `config.h`.
 * @post Execution remains in the emergency LED blink loop until reset.
 * @deprecated none
 */
void mutexWatchdogHandler(void);

#endif


