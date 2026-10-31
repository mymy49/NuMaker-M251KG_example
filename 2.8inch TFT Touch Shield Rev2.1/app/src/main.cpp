/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>
#include <page.h>
#include <util/runtime.h>
#include "memory.h"

int main(void)
 {
	PointerDevice::event_t event;

	// 운영체체 초기화
	initializeYss();

	// 보드 초기화
	initializeBoard();

	initializeMemory();

	pageDisplayLogoId = page.add(pageDisplayLogo, 1024);
	pageDisplayMainId = page.add(pageDisplayMain, 2048);
	pageDisplayTouchCalibrationId = page.add(pageDisplayTouchCalibration, 2048);

	page.play(pageDisplayLogoId);

	thread::delay(5000);
	
	if(memory.touchInitFlag)
		page.play(pageDisplayMainId);
	else
		page.play(pageDisplayTouchCalibrationId);

	while(1)
	{
		thread::yield();
	}
}


