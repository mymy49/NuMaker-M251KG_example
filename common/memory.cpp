#include "memory.h"
#include <yss/instance.h>
#include <bsp.h>

memory_t memory __attribute__ ((aligned (4)));

uint32_t calculateCrc32();

void initializeMemory()
{
	memory = *(memory_t*)flash.getPageAddress(511);

	if(memory.crc32 != calculateCrc32())
	{
		memory.touchInitFlag = false;
		memory.touch.botRightX = 819;
		memory.touch.botRightY = 819;
		memory.touch.topLeftX = 3276;
		memory.touch.topLeftY = 3276;
		memory.touch.width = 240;
		memory.touch.height = 320;

		saveMemory();
	}

	updateMemory();
}

uint32_t calculateCrc32()
{
	crc.resetChecksum();
	return crc.calculate(&memory, sizeof(memory_t) - 4);
}

void updateMemory()
{
	touch.setCalibrationData(memory.touch);
}

void saveMemory()
{
	uint32_t count = (sizeof(memory_t) + 3) >> 2;
	memory.crc32 = calculateCrc32();
	flash.erasePage(511);
	flash.program(511, (uint32_t*)&memory, count);
}

