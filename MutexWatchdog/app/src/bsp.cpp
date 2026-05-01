/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <bsp.h>
#include <yss/instance.h>
#include <yss/debug.h>

void initializeBoard(void)
{
	// LED 초기화
	gpioB.setAsOutput(14);
}

void setLed(bool on)
{
	gpioB.setOutput(14, !on);
}
