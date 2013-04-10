#include "MirfHardwareSpiDriver85.h"
uint8_t MirfHardwareSpiDriver::transfer(uint8_t data){
	return SPI85.transfer(data);
}

void MirfHardwareSpiDriver::begin(){
	SPI85.begin();
	SPI85.setDataMode(SPI_MODE0);
	SPI85.setClockDivider(SPI_2XCLOCK_MASK);
}

void MirfHardwareSpiDriver::end(){
    SPI85.end();
}

MirfHardwareSpiDriver MirfHardwareSpi;
