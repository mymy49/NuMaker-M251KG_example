/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

int main(void)
{
	uint32_t src[4] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD};

	// 운영체체 초기화
	initializeYss();

	// 보드 초기화
	initializeBoard();

	debug_printf("0x%08X\n", crc.calculate(src, sizeof(src)));

	while(1)
	{
		thread::yield();
	}
}


