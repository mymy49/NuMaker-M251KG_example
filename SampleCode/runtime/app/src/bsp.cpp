/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file bsp.cpp
 * @brief Board Support Package implementation for NuMaker-M251KG runtime timer example.
 * @details Provides the board initialization hook for the runtime example. The hardware timers
 *          assigned to `YSS_RUNTIME_TIMER` and `YSS_DELAY_TIMER` in `config.h` are automatically
 *          configured during `initializeYss()`.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include <bsp.h>
#include <yss/instance.h>

/**
 * @details Initializes hardware peripherals specific to the runtime example.
 *          Hardware timers used by the runtime module (`RUNTIME_TIMER0` and `RUNTIME_TIMER1`)
 *          are configured and started directly by `initializeYss()`.
 * @see initializeYss, runtime::getMsec
 * @todo none
 * @bug none
 */
void initializeBoard(void)
{
	// No additional board-specific peripherals required; runtime timers are initialized by initializeYss()
}


