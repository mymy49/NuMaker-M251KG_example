/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG CRC example.
 * @details Implements hardware initialization for the Nuvoton M251 CRC peripheral in 32-bit CRC32 mode.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

/**
 * @details Initializes the hardware CRC engine in CRC32 mode:
 *          - Polynomial mode: CRC-32 (`Crc::CRC_MODE_CRC32`, polynomial 0x04C11DB7)
 *          - Data write width: 32-bit words (`Crc::CRC_DAT_LEN_32BIT`)
 *          - Initial polynomial seed: `0xFFFFFFFF`
 *          - Bit order reversal: Disabled (`false`) for both input write data and output checksum
 *          - Enables the AHB clock for the CRC peripheral, applies the register configuration,
 *            and enables the CRC peripheral interrupt service routine.
 * @see crc, Crc::config_t, Crc::initialize
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
		Crc::CRC_MODE_CRC32,	// mode_t mode: Standard 32-bit CRC-32 polynomial
		Crc::CRC_DAT_LEN_32BIT,	// datalen_t datalen: Process 32-bit words per write cycle
		0xFFFFFFFF,				// uint32_t seed: Standard initial seed value (all ones)
		false,					// bool writeBitOrderReverse: Do not reverse input bit order
		false					// bool checksumBitOrderReverse: Do not reverse output bit order
	};

	// Enable AHB peripheral clock for the hardware CRC controller
	crc.enableClock();
	// Initialize CRC control and seed registers with the specified configuration
	crc.initialize(crcConfig);
	// Enable interrupt handling for the CRC driver instance
	crc.enableInterrupt();
}


