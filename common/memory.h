/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef MEMORY__H_
#define MEMORY__H_

#include <stdint.h>
#include <hal/ResistiveTouch.h>

struct memory_t
{
	bool touchInitFlag;
	ResistiveTouch::calibration_t touch;
	uint32_t crc32;
}__attribute__ ((packed));

extern memory_t memory;

void initializeMemory();

void updateMemory();

void saveMemory();

#endif

