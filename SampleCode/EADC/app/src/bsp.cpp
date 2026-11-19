/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG EADC example.
 * @details Configures GPIO Port B pins 0 and 1 for analog multiplexing and sets up the
 *          14-bit Enhanced ADC (EADC) driver with internal digital low-pass filtering.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>

/**
 * @details Configures PB0 and PB1 as analog input pins (`Gpio::PB0_EADC0_CH0` and `Gpio::PB1_EADC0_CH1`).
 *          Enables the EADC peripheral clock, initializes the EADC driver for 2 active channels,
 *          and enables EADC conversion completion interrupts. Registers channels 0 and 1 with
 *          14-bit output resolution (`Adc::RES_BIT14`) and level-14 software low-pass filtering
 *          (`Adc::LPF_LV14`), then triggers continuous background conversions.
 * @see eadc, gpioB, Adc::add, Adc::convert
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// --------------------------------------------------
	// 1. Configure GPIO pins for EADC input channels
	// --------------------------------------------------
	// Route PB0 to EADC0 Channel 0 and disable digital input path
	gpioB.setAsAltFunc(0, Gpio::PB0_EADC0_CH0);
	// Route PB1 to EADC0 Channel 1 and disable digital input path
	gpioB.setAsAltFunc(1, Gpio::PB1_EADC0_CH1);

	// --------------------------------------------------
	// 2. Initialize EADC peripheral
	// --------------------------------------------------
	// Enable APB clock for the EADC peripheral
	eadc.enableClock();
	// Allocate and initialize EADC internal state for 2 conversion channels
	eadc.initialize(2);
	// Enable EADC NVIC interrupt for asynchronous sample accumulation
	eadc.enableInterrupt();

	// --------------------------------------------------
	// 3. Register conversion channels with LPF & 14-bit resolution
	// --------------------------------------------------
	// Register Channel 0 with Level-14 low-pass filter and 14-bit output scaling
	eadc.add(0, Adc::LPF_LV14, Adc::RES_BIT14);
	// Register Channel 1 with Level-14 low-pass filter and 14-bit output scaling
	eadc.add(1, Adc::LPF_LV14, Adc::RES_BIT14);

	// Start continuous background A/D conversions across all registered channels
	eadc.convert(true);
}


