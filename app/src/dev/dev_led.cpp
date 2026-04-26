#include <dev/led.h>
#include <yss/instance.h>

namespace Led 
{
	void initialize(void)
	{
		gpioB.setAsOutput(14);

		red(false);
	}

	void red(bool on)
	{
		gpioB.setOutput(14, !on);
	}
}

