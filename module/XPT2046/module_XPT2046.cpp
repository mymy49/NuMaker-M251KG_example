#include "XPT2046.h"
#include <yss/debug.h>

static const Spi::specification_t gSpiSpec =
{
	Spi::CLOCK_MODE_MODE0,	//uint8_t mode;
	2000000,				//uint32_t maxFreq;
	Spi::BIT_BIT8			//uint8_t bit;
};

XPT2046::XPT2046()
{
	mSpi = nullptr;
}

error_t XPT2046::initialize(config_t config)
{
	mSpi = &config.spi;
	mCsPin = config.chipSelect;
	mBusyPin = config.busy;
	mIsrPin = config.isr;

	activateTrigger(512);
	mIsrPin.port->setGpioInterrupt(mIsrPin.pin, Gpio::EDGE_FALLING, getTriggerId());
	mIsrPin.port->enableInterrupt(true);
	
	mCsPin.port->setOutput(mCsPin.pin, true);
	thread::delay(100);
	
	convert(mConXpos);
	
	return error_t::ERROR_NONE;
}

uint16_t XPT2046::convert(XPT2046_Control_t control)
{
	uint16_t data;

	mSpi->lock();
	mSpi->setSpecification(gSpiSpec);
	mCsPin.port->setOutput(mCsPin.pin, false);
	mSpi->exchange(control.byte);
	thread::delayUs(100);
	data = mSpi->exchange(0) << 5;
	data |= mSpi->exchange(0) >> 3;
	mCsPin.port->setOutput(mCsPin.pin, true);
	mSpi->unlock();

	return data;
}

void XPT2046::trigger()
{
	thread::delayUs(100);
	uint16_t x = calculateX(convert(mConXpos));
	uint16_t y = calculateY(convert(mConYpos));

	pushDownEvent(x, y);

	while(!mIsrPin.port->getInputData(mIsrPin.pin))
	{
		x = calculateX(convert(mConXpos));
		y = calculateY(convert(mConYpos));
		
		pushDragEvent(x, y);
		thread::yield();
	}
	pushUpEvent(x, y);
}
