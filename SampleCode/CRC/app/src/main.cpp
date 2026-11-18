/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file main.cpp
 * @brief Main application demonstration for hardware CRC32 calculation.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

/**
 * @details Initializes yss OS kernel and board hardware. Feeds a sample 4-word (16-byte)
 *          array into the hardware CRC engine and outputs the resulting CRC32 checksum
 *          via debug_printf. Then enters a thread yield loop.
 * @see initializeYss, initializeBoard, crc
 * @todo none
 * @bug none
 */
int main(void)
{
	uint32_t src[4] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD};

	// --------------------------------------------------
	// 1. Initialize operating system kernel
	// --------------------------------------------------
	initializeYss();

	// --------------------------------------------------
	// 2. Initialize board hardware and CRC engine
	// --------------------------------------------------
	initializeBoard();

	// Calculate and print CRC32 checksum over the sample array
	debug_printf("0x%08X\n", crc.calculate(src, sizeof(src)));

	// Main thread loop
	while(1)
	{
		thread::yield();
	}
}



