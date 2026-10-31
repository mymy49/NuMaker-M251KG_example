/*
 * Copyright (c) 2026 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef	YSS_MODULE_XPT2046__H_
#define	YSS_MODULE_XPT2046__H_

#include <hal/ResistiveTouch.h>
#include <drv/Spi.h>
#include <drv/Gpio.h>
#include <yss/Trigger.h>

class XPT2046 : public ResistiveTouch, protected Trigger
{
public :
	struct config_t
	{
		Spi &spi;
		pin_t chipSelect;
		pin_t busy;
		pin_t isr;
	};

	XPT2046();

	error_t initialize(config_t config);
	
protected :
	union XPT2046_Control_t
	{
	    struct 
		{
	        uint8_t pd      : 2;    // Bit 0: Power-Down 비트 0
	        uint8_t ser_dfr : 1;    // Bit 2: Single-Ended(1) / Differential(0) 모드
	        uint8_t mode    : 1;    // Bit 3: 분해능 (0 = 12-bit, 1 = 8-bit)
	        uint8_t channel : 3;    // Bit 4: 채널 선택 A0
	        uint8_t s       : 1;    // Bit 7: Start 비트 (항상 1이어야 함)
	    } bits;

	    uint8_t byte;               // SPI로 전송할 전체 1바이트 데이터
	};

	const XPT2046_Control_t mConXpos = 
	{
		0,	//uint8_t pd      : 2;
		0,	//uint8_t ser_dfr : 1;
		0,	//uint8_t mode    : 1;
		5,	//uint8_t channel : 3;
		1	//uint8_t s       : 1;
	};

	const XPT2046_Control_t mConYpos = 
	{
		0,	//uint8_t pd      : 2;
		0,	//uint8_t ser_dfr : 1;
		0,	//uint8_t mode    : 1;
		1,	//uint8_t channel : 3;
		1	//uint8_t s       : 1;
	};

	uint16_t convert(XPT2046_Control_t control);

private :
	Spi *mSpi;
	pin_t mCsPin;
	pin_t mBusyPin;
	pin_t mIsrPin;

	void trigger() override;
};

#endif

