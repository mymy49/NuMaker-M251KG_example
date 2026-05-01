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
	// CRC 초기화
	Crc::config_t crcConfig =
	{
		Crc::CRC_MODE_CRC32,	//mode_t mode;
		Crc::CRC_DAT_LEN_32BIT,	//datalen_t datalen;
		0xFFFFFFFF,				//uint32_t seed;
		false,					//bool writeBitOrderReverse;
		false					//bool checksumBitOrderReverse;
	};

	crc.enableClock();
   	crc.initialize(crcConfig);
	crc.enableInterrupt();
}

