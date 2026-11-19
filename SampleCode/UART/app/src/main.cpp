/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Demonstration of interrupt-driven UART echo communication and LED trigger.
 * @details Receives serial data over UART0 via interrupt-buffered ring queue, echoes received
 *          bytes back to the sender, and triggers a non-blocking 20 ms activity LED pulse.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Trigger task handler that blinks the red LED for 20 milliseconds to indicate
 *          incoming UART reception activity without blocking the main UART echo loop.
 * @see Led::red, thread::delay, trigger::run
 * @todo none
 * @bug none
 */
void trigger_blinkLed(void)
{
	// Turn ON the activity indicator LED
	Led::red(true);
	// Hold the LED illuminated for 20 ms while yielding CPU to other threads
	thread::delay(20);
	// Turn OFF the activity indicator LED
	Led::red(false);
}

/**
 * @details Initializes the yss OS kernel and board hardware. Registers an asynchronous
 *          trigger task (`trigger_blinkLed`) for LED activity blinking. In the main polling loop,
 *          continuously inspects the UART0 received byte count; whenever one or more bytes arrive,
 *          it fires the blink trigger, echoes the contiguous buffer segment back through UART0,
 *          and releases the processed bytes from the internal ring buffer.
 * @see initializeYss, initializeBoard, uart0, trigger::add, trigger::run, trigger_blinkLed
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

	// Register LED blink action as an OS trigger task with a 512-byte stack
	id = trigger::add(trigger_blinkLed, 512);

	// UART echo loop
	while(1)
	{
		// Query number of contiguous bytes currently available in the UART0 receive ring buffer
		count = uart0.getRxCount();

		if(count > 0)
		{
			// Asynchronously fire the 20 ms activity indicator LED blink trigger
			trigger::run(id);

			// Retrieve pointer to the current read position in the UART0 receive ring buffer
			buf = uart0.getRxBuffer();

			// Echo the received byte block back to the UART0 transmitter
			uart0.send(buf, count);

			// Advance the internal ring buffer tail pointer by the number of processed bytes
			uart0.releaseRxBuffer(count); 
		}
		else
		{
			// Yield processor timeslice to other threads if no serial data has arrived
			thread::yield();
		}
	}
}



