/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file memory.cpp
 * @brief Implementation of persistent memory management routines.
 * @details Implements non-volatile configuration loading, hardware CRC32 checksum validation,
 *          peripheral synchronization, and internal flash programming on page 511 of the M251 APROM.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "memory.h"
#include <yss/instance.h>
#include <bsp.h>

/**
 * @brief 4-byte aligned global memory instance holding runtime configuration data.
 */
memory_t memory __attribute__ ((aligned (4)));

/**
 * @details Reads the stored `memory_t` structure from the last sector of internal flash memory
 *          (page index 511). Verifies data integrity by comparing the stored CRC32 against
 *          the newly calculated checksum. If the checksum mismatches or the flash page is unprogrammed,
 *          it resets touch calibration parameters to their default uncalibrated boundaries and
 *          immediately writes them back to flash. Finally, calls `updateMemory()` to propagate
 *          the loaded parameters to active hardware drivers.
 * @see calculateCrc32, updateMemory, saveMemory, flash
 * @todo none
 * @bug none
 */
void initializeMemory()
{
	// Dereference the physical flash memory address of page 511 to copy stored settings into RAM
	memory = *(memory_t*)flash.getPageAddress(511);

	// Validate memory contents by comparing the stored CRC32 with the newly computed CRC32
	if(memory.crc32 != calculateCrc32())
	{
		// Checksum mismatch detected: restore default uncalibrated touch screen boundaries
		memory.touchInitFlag = false;
		memory.touch.botRightX = 0;
		memory.touch.botRightY = 0;
		memory.touch.topLeftX = 4095;
		memory.touch.topLeftY = 4095;
		memory.touch.width = 240;
		memory.touch.height = 320;

		// Persist the initialized default configuration structure to flash page 511
		saveMemory();
	}

	// Apply the validated configuration parameters to registered peripheral drivers
	updateMemory();
}

/**
 * @details Resets the hardware CRC peripheral engine to its configured seed value and calculates
 *          the 32-bit checksum over the `memory_t` structure excluding the trailing 4-byte `crc32`
 *          member field itself.
 * @see crc, Crc::resetChecksum, Crc::calculate
 * @todo none
 * @bug none
 */
uint32_t calculateCrc32()
{
	// Reset hardware CRC accumulator register to its initial polynomial seed (0xFFFFFFFF)
	crc.resetChecksum();

	// Compute and return CRC32 over all payload bytes preceding the 4-byte crc32 field
	return crc.calculate(&memory, sizeof(memory_t) - 4);
}

/**
 * @details Checks whether the 2.8-inch TFT touch shield module is enabled at compile time,
 *          and if so, updates the XPT2046 resistive touch controller driver with the calibration
 *          matrix stored in the global `memory` structure.
 * @see touch, ResistiveTouch::setCalibrationData
 * @todo none
 * @bug none
 */
void updateMemory()
{
#if defined(__2p8INCH_TFT_TOUCH_SHIELD_REV2p1__)
	// Apply the loaded calibration matrix to the XPT2046 resistive touch driver instance
	touch.setCalibrationData(memory.touch);
#endif
}

/**
 * @details Computes a fresh CRC32 value over the configuration payload, stores it
 *          in `memory.crc32`, erases flash page 511, and programs the updated data words
 *          into internal flash memory in 32-bit word units.
 * @see calculateCrc32, flash, Flash::erasePage, Flash::program
 * @todo none
 * @bug none
 */
void saveMemory()
{
	// Calculate the number of 32-bit words required to store memory_t (rounding up to 4-byte boundary)
	uint32_t count = (sizeof(memory_t) + 3) >> 2;

	// Compute and store the updated CRC32 checksum in the structure before flashing
	memory.crc32 = calculateCrc32();

	// Erase flash page 511 so bits can be programmed from 1 to 0
	flash.erasePage(511);

	// Program the 4-byte aligned memory structure into flash page 511 as 32-bit words
	flash.program(511, (uint32_t*)&memory, count);
}


