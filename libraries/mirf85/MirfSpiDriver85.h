#ifndef __MIRF_SPI_DRIVER85
#define __MIRF_SPI_DRIVER85

extern "C" {
	#include <string.h>
	#include <inttypes.h>
}

class MirfSpiDriver {
	public:
		virtual uint8_t transfer(uint8_t data);

		virtual void begin();
		virtual void end();
};

#endif
