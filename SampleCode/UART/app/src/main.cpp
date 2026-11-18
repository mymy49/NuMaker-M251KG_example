/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Demonstration of interrupt-driven UART echo communication and LED trigger.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Trigger task handler that blinks the red LED for 20 milliseconds to indicate
 *          incoming UART reception activity.
 * @see Led::red, thread::delay
 * @todo none
 * @bug none
 */
void trigger_blinkLed(void)
{
	Led::red(true);
	thread::delay(20);
	Led::red(false);
}

/**
 * @details Initializes the yss OS kernel and board hardware. Registers an asynchronous
 *          trigger task for LED blinking. In the main polling loop, continuously inspects
 *          UART0 received byte count; whenever data arrives, fires the blink trigger,
 *          echoes the received bytes back through UART0, and releases the ring buffer.
 * @see initializeYss, initializeBoard, uart0, trigger::add, trigger::run
 * @todo none
 * @bug none
 */
int main(void)
{
	uint32_t count;
	int8_t *buf;
	triggerId_t id;

	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and UART0
	// --------------------------------------------------
	initializeBoard();

	// Register LED blink action as an OS trigger task (stack size: 512 bytes)
	id = trigger::add(trigger_blinkLed, 512);

	// UART echo loop
	while(1)
	{
		// Query number of bytes currently stored in UART0 receive buffer
		count = uart0.getRxCount();

		if(count > 0)
		{
			// Activate activity indicator LED
			trigger::run(id);

			// Retrieve pointer to received data buffer
			buf = uart0.getRxBuffer();

			// Echo data back to sender
			uart0.send(buf, count);

			// Advance internal read pointer
			uart0.releaseRxBuffer(count); 
		}
		else
		{
			// Yield processor timeslice if no data has arrived
			thread::yield();
		}
	}
}



