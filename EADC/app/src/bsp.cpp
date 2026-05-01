/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#include <bsp.h>
#include <yss/instance.h>

void initializeBoard(void)
{
	// EADC 초기화
	gpioB.setAsAltFunc(0, Gpio::PB0_EADC0_CH0);
	gpioB.setAsAltFunc(1, Gpio::PB1_EADC0_CH1);

	eadc.enableClock();
	eadc.initialize(2);
	eadc.enableInterrupt();

	eadc.add(0, Adc::LPF_LV14, Adc::RES_BIT14);
	eadc.add(1, Adc::LPF_LV14, Adc::RES_BIT14);

	eadc.convert(true);
}

