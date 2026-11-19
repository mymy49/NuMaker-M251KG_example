/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for hardware CRC32 calculation.
 * @details Demonstrates computing a 32-bit Cyclic Redundancy Check (CRC32) over an array of
 *          32-bit data words using the Nuvoton M251 hardware CRC accelerator in yss OS.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes the yss OS kernel and board hardware (including the CRC peripheral).
 *          Feeds a sample 4-word (16-byte) test pattern array (`src`) into the hardware CRC
 *          engine via `crc.calculate()` and prints the resulting 32-bit hexadecimal checksum
 *          to the SWD debug terminal (`debug_printf`). Afterward, enters an infinite loop
 *          yielding CPU time to the RTOS scheduler.
 * @see initializeYss, initializeBoard, crc, Crc::calculate, thread::yield
 * @todo none
 * @bug none
 */
int main(void)
{
	// Sample 16-byte (Four 32-bit words) data payload for CRC32 checksum verification
	uint32_t src[4] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD};

	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and CRC engine
	// --------------------------------------------------
	initializeBoard();

	// Calculate hardware CRC32 checksum over the 16-byte sample buffer and print in 8-digit hex format
	debug_printf("0x%08X\n", crc.calculate(src, sizeof(src)));

	// Main thread loop: yield CPU time slice to other RTOS threads
	while(1)
	{
		thread::yield();
	}
}



