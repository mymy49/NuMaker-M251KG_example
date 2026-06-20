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

int main(void)
{
	// 운영체체 초기화
	initializeYss();

	// 보드 초기화
	initializeBoard();

	pageDisplayLogoId = page.add(pageDisplayLogo);
	pageDisplayMainId = page.add(pageDisplayMain);

	page.play(pageDisplayLogoId, 1024);

	thread::delay(5000);

	page.play(pageDisplayMainId);

	while(1)
	{
		thread::yield();
	}
}


