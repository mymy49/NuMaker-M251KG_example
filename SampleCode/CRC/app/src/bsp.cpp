/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG CRC example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes the hardware CRC engine in CRC32 mode:
 *          - Data length: 32-bit words
 *          - Initial polynomial seed: 0xFFFFFFFF
 *          - Bit reversal: None (standard bit order for both write and checksum)
 *          - Enables CRC peripheral clock and interrupt service routine.
 * @see crc
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure Hardware CRC Engine
	// --------------------------------------------------
	Crc::config_t crcConfig =
	{
		Crc::CRC_MODE_CRC32,	// mode_t mode;
		Crc::CRC_DAT_LEN_32BIT,	// datalen_t datalen;
		0xFFFFFFFF,				// uint32_t seed;
		false,					// bool writeBitOrderReverse;
		false					// bool checksumBitOrderReverse;
	};

	// Enable clock, initialize registers, and enable CRC interrupts
	crc.enableClock();
	crc.initialize(crcConfig);
	crc.enableInterrupt();
}


