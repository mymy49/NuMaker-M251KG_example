/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

void thread_testMutexWatchdog(void);

Mutex gMutex;

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

int main(void)
{
	// 운영체체 초기화
	initializeYss();

	// 보드 초기화
	initializeBoard();

	thread::add(thread_testMutexWatchdog, 512);

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

void thread_testMutexWatchdog(void)
{
	thread::delay(5000);
	gMutex.lock();
}