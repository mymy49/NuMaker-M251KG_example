/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file memory.h
 * @brief Non-volatile memory management and touch calibration data storage.
 * @details Declares the persistent configuration structure (`memory_t`) and flash storage
 *          management routines used to load, verify, apply, and save system parameters
 *          protected by a hardware CRC32 checksum.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#ifndef MEMORY__H_
#define MEMORY__H_

#include <stdint.h>
#include <hal/ResistiveTouch.h>

/**
 * @struct memory_t
 * @brief Persistent configuration data structure stored in internal flash memory.
 * @details Stores system-wide non-volatile configuration parameters including touch screen calibration
 *          coordinates (inherited from `ResistiveTouch::calibration_t` in the yss OS HAL module) and
 *          an integrity verification checksum (`crc32`). Packed to ensure a deterministic binary layout
 *          across flash read and write operations.
 *
 * Example usage:
 * @code
 * initializeMemory();
 * if (!memory.touchInitFlag)
 * {
 *     // Run touch calibration procedure
 *     memory.touch.topLeftX = 3500;
 *     memory.touch.topLeftY = 3600;
 *     memory.touch.botRightX = 500;
 *     memory.touch.botRightY = 450;
 *     memory.touchInitFlag = true;
 *     saveMemory();
 * }
 * @endcode
 */
struct memory_t
{
	bool touchInitFlag;                     ///< Flag indicating whether touch calibration has been completed.
	ResistiveTouch::calibration_t touch;    ///< Resistive touch calibration coordinates and display dimensions.
	uint32_t crc32;                         ///< CRC32 checksum for validating data integrity in flash memory.
} __attribute__ ((packed));

/**
 * @brief Global memory structure holding active configuration loaded from flash.
 * @details Aligned on a 4-byte boundary in RAM to support 32-bit word programming into internal APROM flash.
 */
extern memory_t memory;

/**
 * @brief Calculates the CRC32 checksum of the active memory configuration structure.
 * @param[in] none
 * @return Computed 32-bit CRC checksum value over `memory_t` (excluding the trailing `crc32` field).
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning The hardware CRC peripheral must be initialized in CRC32 mode before calling this function.
 * @note Resets the hardware CRC seed prior to computing the checksum over `sizeof(memory_t) - 4` bytes.
 * @pre Hardware CRC peripheral clock and configuration must be enabled.
 * @post Hardware CRC engine contains the computed checksum for the current `memory` contents.
 * @deprecated none
 */
uint32_t calculateCrc32();

/**
 * @brief Initializes non-volatile memory and verifies data integrity.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning If the CRC32 check fails, default calibration values are restored and saved to flash.
 * @note Reads configuration from flash page 511.
 * @pre Flash and CRC hardware peripherals must be initialized.
 * @post The global memory variable is initialized and touch parameters are applied.
 * @deprecated none
 */
void initializeMemory();

/**
 * @brief Updates hardware peripherals with active configuration from memory.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning none
 * @note Applies touch calibration parameters to the touch controller instance if available.
 * @pre initializeMemory() should be called first.
 * @post Touch controller calibration data is updated.
 * @deprecated none
 */
void updateMemory();

/**
 * @brief Saves the current memory structure to flash memory with calculated CRC32.
 * @param[in] none
 * @return none
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 * @warning Erases flash page 511 before writing. Ensure interrupts or concurrent writes do not interfere.
 * @note Calculates CRC32 over memory_t (excluding the crc32 field) and writes the page.
 * @pre Flash controller must be operational.
 * @post Flash page 511 contains the updated memory_t structure and valid CRC.
 * @deprecated none
 */
void saveMemory();

#endif


