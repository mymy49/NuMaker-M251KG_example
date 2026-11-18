/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG EADC example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>

/**
 * @details Configures PB0 and PB1 as analog input pins (EADC0 CH0 and CH1).
 *          Initializes the EADC peripheral for 2 channels with interrupt support,
 *          configures 14-bit resolution and level-14 digital low-pass filtering,
 *          and triggers continuous analog-to-digital conversions.
 * @see eadc, gpioB
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure GPIO pins for EADC input channels
	// --------------------------------------------------
	gpioB.setAsAltFunc(0, Gpio::PB0_EADC0_CH0);
	gpioB.setAsAltFunc(1, Gpio::PB1_EADC0_CH1);

	// --------------------------------------------------
	// 2. Initialize EADC peripheral
	// --------------------------------------------------
	eadc.enableClock();
	eadc.initialize(2);
	eadc.enableInterrupt();

	// --------------------------------------------------
	// 3. Register conversion channels with LPF & 14-bit resolution
	// --------------------------------------------------
	eadc.add(0, Adc::LPF_LV14, Adc::RES_BIT14);
	eadc.add(1, Adc::LPF_LV14, Adc::RES_BIT14);

	// Start continuous background conversions
	eadc.convert(true);
}


