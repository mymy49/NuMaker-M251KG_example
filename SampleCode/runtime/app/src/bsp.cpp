/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG runtime timer example.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>

/**
 * @details Initializes hardware peripherals specific to the runtime example.
 *          Hardware timers used by the runtime module (TIMER0 and TIMER1) are
 *          managed directly by the yss OS initialization layer.
 * @see initializeYss
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{

}


