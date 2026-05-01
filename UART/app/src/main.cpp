/*
 * Copyright (c) 2025 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <yss.h>
#include <bsp.h>
#include <yss/debug.h>

void trigger_blinkLed(void)
{
	Led::red(true);
	thread::delay(20);
	Led::red(false);
}

int main(void)
{
	uint32_t count;
	int8_t *buf;
	triggerId_t id;

	// 운영체체 초기화
	initializeYss();

	// 보드 초기화
	initializeBoard();

	id = trigger::add(trigger_blinkLed, 512);

	while(1)
	{
		count = uart0.getRxCount();

		if(count > 0)
		{
			trigger::run(id);
			buf = uart0.getRxBuffer();
			uart0.send(buf, count);
			uart0.releaseRxBuffer(count); 
		}
		else
			thread::yield();
	}
}


