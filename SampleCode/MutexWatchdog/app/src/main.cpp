/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Demonstration of RTOS Mutex Watchdog mechanism to detect deadlock or prolonged locks.
 * @details Illustrates how yss OS monitors mutex lock contention and invokes `mutexWatchdogHandler()`
 *          when a thread waits on a locked `Mutex` longer than `THREAD_WATCHDOG_OVERFLOW_TIME` (1000 ms).
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @brief Shared mutex instance protected by the yss OS mutex watchdog supervisor.
 */
Mutex gMutex;

/**
 * @details Handler invoked automatically by the yss OS scheduler when a thread attempts to lock
 *          a mutex that remains held beyond the `THREAD_WATCHDOG_OVERFLOW_TIME` (1000 ms) threshold.
 *          Overrides the default weak reset handler and blinks the indicator LED in a slower
 *          500 ms emergency cycle (compared to the normal 100 ms cycle) to signal deadlock detection.
 * @see gMutex, setLed, thread::delay
 * @todo none
 * @bug none
 */
void mutexWatchdogHandler(void)
{
	// Emergency deadlock indication loop: slow 1 Hz blink (500 ms ON, 500 ms OFF)
	while(true)
	{
		// Turn ON the fault indicator LED and wait 500 ms
		setLed(true);	
		thread::delay(500);

		// Turn OFF the fault indicator LED and wait 500 ms
		setLed(false);
		thread::delay(500);		
	}
}

/**
 * @details Initializes the yss OS kernel and board hardware. Spawns a background worker
 *          thread (`thread_testMutexWatchdog`) and repeatedly toggles the LED while locking
 *          and unlocking `gMutex` at fast 100 ms intervals. When the worker thread locks `gMutex`
 *          indefinitely after 5 seconds, the main thread's next `gMutex.lock()` call blocks for
 *          more than 1000 ms and triggers `mutexWatchdogHandler()`.
 * @see initializeYss, initializeBoard, thread_testMutexWatchdog, mutexWatchdogHandler, gMutex
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

	// Spawn the deadlock-inducing tester thread with a 512-byte stack
	thread::add(thread_testMutexWatchdog, 512);

	// Normal operation loop: fast 5 Hz blink (100 ms ON, 100 ms OFF) under mutex protection
	while(true)
	{
		// Acquire shared mutex, turn LED ON, and immediately release mutex
		gMutex.lock();
		setLed(true);
		gMutex.unlock();
		
		// Wait 100 ms before toggling LED state
		thread::delay(100);

		// Acquire shared mutex, turn LED OFF, and immediately release mutex
		gMutex.lock();
		setLed(false);
		gMutex.unlock();
		
		// Wait 100 ms before next cycle
		thread::delay(100);
	}
}

/**
 * @details Waits 5000 ms after system startup to allow normal fast LED blinking to be observed,
 *          and then locks `gMutex` without ever unlocking it. This simulates a hung thread holding
 *          a critical resource and causes the main thread to trip the 1000 ms mutex watchdog timeout.
 * @see gMutex, Mutex::lock, mutexWatchdogHandler
 * @todo none
 * @bug none
 */
void thread_testMutexWatchdog(void)
{
	// Delay 5 seconds during normal operation, then permanently lock the shared mutex without unlocking
	thread::delay(5000);
	gMutex.lock();
}
