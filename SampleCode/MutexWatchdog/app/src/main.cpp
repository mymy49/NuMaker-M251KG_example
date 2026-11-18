/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Demonstration of RTOS Mutex Watchdog mechanism to detect deadlock or prolonged locks.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @brief Thread entry point designed to induce a mutex deadlock / watchdog trigger.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Deliberately holds gMutex without releasing to trigger mutexWatchdogHandler.
 * @note Waits 5 seconds before locking the shared mutex.
 * @pre gMutex must be initialized.
 * @post gMutex is locked indefinitely by this thread.
 * @deprecated none
 */
void thread_testMutexWatchdog(void);

/// Shared mutex protected by the yss OS mutex watchdog
Mutex gMutex;

/**
 * @details Handler invoked automatically when a thread locks a mutex and fails to release
 *          it within the THREAD_WATCHDOG_OVERFLOW_TIME threshold. Overrides the default reset
 *          action and blinks the indicator LED in an emergency 500ms cycle.
 * @see gMutex, setLed
 * @todo none
 * @bug none
 */
void mutexWatchdogHandler(void)
{
	while(true)
	{
		setLed(true);	
		thread::delay(500);

		setLed(false);
		thread::delay(500);		
	}
}

/**
 * @details Initializes the yss OS kernel and board hardware. Spawns a background worker
 *          thread (thread_testMutexWatchdog) and repeatedly toggles the LED while locking
 *          and unlocking gMutex with 100ms intervals. When the worker thread locks gMutex
 *          indefinitely after 5 seconds, the watchdog triggers mutexWatchdogHandler().
 * @see initializeYss, initializeBoard, thread_testMutexWatchdog, mutexWatchdogHandler
 * @todo none
 * @bug none
 */
int main(void)
{
	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and LED
	// --------------------------------------------------
	initializeBoard();

	// Spawn tester thread with 512 bytes stack
	thread::add(thread_testMutexWatchdog, 512);

	// Periodic toggle loop under mutex protection
	while(true)
	{
		gMutex.lock();
		setLed(true);
		gMutex.unlock();
		
		thread::delay(100);

		gMutex.lock();
		setLed(false);
		gMutex.unlock();
		
		thread::delay(100);
	}
}

/**
 * @details Waits 5000ms after startup and locks gMutex without unlocking it, causing
 *          subsequent lock requests in the main thread to wait and trip the 1000ms watchdog timeout.
 * @see gMutex
 * @todo none
 * @bug none
 */
void thread_testMutexWatchdog(void)
{
	// Delay 5 seconds then permanently lock the shared mutex
	thread::delay(5000);
	gMutex.lock();
}
