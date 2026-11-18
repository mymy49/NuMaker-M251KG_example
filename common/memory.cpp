/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file memory.cpp
 * @brief Implementation of persistent memory management routines.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "memory.h"
#include <yss/instance.h>
#include <bsp.h>

/// 4-byte aligned global memory instance
memory_t memory __attribute__ ((aligned (4)));

/**
 * @brief Internal helper to calculate CRC32 checksum of the memory structure.
 * @return Computed 32-bit CRC checksum value.
 */
uint32_t calculateCrc32();

/**
 * @details Reads the stored memory_t structure from the last sector of internal flash memory
 *          (page index 511). Verifies data integrity by comparing the stored CRC32 against
 *          the calculated checksum. If the checksum mismatches or the data is corrupted,
 *          it resets touch parameters to their default values and immediately writes them back
 *          to flash. Finally, calls updateMemory() to propagate parameters to active drivers.
 * @see calculateCrc32, updateMemory, saveMemory, flash
 * @todo none
 * @bug none
 */
void initializeMemory()
{
	// Read saved configuration directly from internal flash page 511
	memory = *(memory_t*)flash.getPageAddress(511);

	// Validate memory contents using CRC32 checksum
	if(memory.crc32 != calculateCrc32())
	{
		// Checksum mismatch: restore default touch calibration boundaries
		memory.touchInitFlag = false;
		memory.touch.botRightX = 0;
		memory.touch.botRightY = 0;
		memory.touch.topLeftX = 4095;
		memory.touch.topLeftY = 4095;
		memory.touch.width = 240;
		memory.touch.height = 320;

		// Persist default configuration to flash
		saveMemory();
	}

	// Apply loaded configuration to peripherals
	updateMemory();
}

/**
 * @details Resets the hardware CRC peripheral engine and calculates the 32-bit checksum
 *          over the memory_t structure excluding the trailing 4-byte crc32 field itself.
 * @see crc
 * @todo none
 * @bug none
 */
uint32_t calculateCrc32()
{
	// Reset hardware CRC accumulator to initial seed
	crc.resetChecksum();
	// Calculate CRC excluding the last 4 bytes (the crc32 member itself)
	return crc.calculate(&memory, sizeof(memory_t) - 4);
}

/**
 * @details Checks whether the 2.8-inch TFT touch shield is enabled, and if so,
 *          updates the XPT2046 resistive touch controller driver with the calibration
 *          matrix stored in the global memory structure.
 * @see touch, ResistiveTouch::setCalibrationData
 * @todo none
 * @bug none
 */
void updateMemory()
{
#if defined(__2p8INCH_TFT_TOUCH_SHIELD_REV2p1__)
	// Apply calibration matrix to the resistive touch driver
	touch.setCalibrationData(memory.touch);
#endif
}

/**
 * @details Computes a fresh CRC32 value over the configuration structure, stores it
 *          in memory.crc32, erases flash page 511, and programs the updated data words
 *          into internal flash memory.
 * @see calculateCrc32, flash
 * @todo none
 * @bug none
 */
void saveMemory()
{
	// Convert byte length to 32-bit word count (rounded up)
	uint32_t count = (sizeof(memory_t) + 3) >> 2;

	// Calculate and store updated CRC32 checksum
	memory.crc32 = calculateCrc32();

	// Erase page 511 before writing
	flash.erasePage(511);

	// Write memory structure to flash as 32-bit words
	flash.program(511, (uint32_t*)&memory, count);
}


