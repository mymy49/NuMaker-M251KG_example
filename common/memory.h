/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file memory.h
 * @brief Non-volatile memory management and touch calibration data storage.
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
 *          coordinates and an integrity verification checksum (CRC32). Packed to ensure consistent binary layout.
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
 */
extern memory_t memory;

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


