/*

This is an attiny85 example code for the nRF24L01 that can communicate with RF24 library

All the support files and libraries for the attiny85 for nRF24L01 is at repo listed  below

 *  repo   : https://github.com/stanleyseow/attiny-nRF24L01
 *  Author : Stanley Seow
 *  e-mail : stanleyseow@gmail.com
 *  date   : 11th Apr 2013
 
 Some default values to take note when using this mirf85/spi85 library
 
 - changed the default of CRC8 to CRC16 ( mirf85.h )
 - added rf_setup to Nrf24l::config  ( mirf85.cpp / mirf85.h )
 - added some defines for Dynamic Payload in the header files ( nRF24L0185.h )
 - node addressing is similar to RF24 libs but the bytes are flipped 
 
   byte TADDR[] = {0xe3, 0xf0, 0xf0, 0xf0, 0xf0}; will matches receiver node of RF24 below 
   const uint64_t pipes[2] = { 0x7365727631LL, 0xF0F0F0F0E3LL };
 
 The repo for the RF24 lib is at https://github.com/stanleyseow/RF24/

*/


#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <SPI85.h>
#include <Mirf85.h>
#include <nRF24L0185.h>
#include <MirfHardwareSpiDriver85.h>


// Pinout
// CE   - Pin 2 / D3 / nRF3
// CSN  - Pin 3 / D4 / nRF4
// GND  - Pin 4
// MISO - Pin 5 / D0 / nRF7
// MOSI - Pin 6 / D1 / nRF6
// SCK  - Pin 7 / D2 / nRF5
// Vcc  - Pin 8 ( 3.3V )

int bufferSize = 0;
byte buffer[] = { 'H', 'e', 'l', 'l', 'o','!' };

#define CE  PB3 //ATTiny
#define CSN PB4 //ATTiny

void setup(){
  Mirf.cePin = CE;
  Mirf.csnPin = CSN;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // This address is compatible with my example of rpi-hub or nRF24_Arduino_as_hub
  // at repo https://github.com/stanleyseow/RF24/examples/ 
  
  byte RADDR[] = {0xe2, 0xf0, 0xf0, 0xf0, 0xf0};
  byte TADDR[] = {0xe3, 0xf0, 0xf0, 0xf0, 0xf0};
 
  Mirf.payload = sizeof(buffer);
  
  Mirf.channel = 0x4c; // Same as rpi-hub and sendto_hub
  Mirf.rfsetup = 0x06; // 1Mbps, Max power
  Mirf.setRADDR(RADDR);  
  Mirf.setTADDR(TADDR);
  Mirf.config();
  
  // Enable dynamic payload 
  Mirf.configRegister( FEATURE, 1<<EN_DPL ); 
  Mirf.configRegister( DYNPD, 1<<DPL_P0 | 1<<DPL_P1 | 1<<DPL_P2 | 1<<DPL_P3 | 1<<DPL_P4 | 1<<DPL_P5 ); 
    
}

void loop(){
  
  uint8_t sent = false;
  
  Mirf.send(buffer);
  while( Mirf.isSending() )
  {
    delay(1);
    sent = true; // Sent success
  }

  delay(400);
  if (sent) {   // Blink CE ( pin 2), 3 times 
      digitalWrite(3,HIGH);  
      delay(100);
      digitalWrite(3,LOW);  
      delay(100);
      digitalWrite(3,HIGH); 
      delay(100);
      digitalWrite(3,LOW);  
      delay(100);
      digitalWrite(3,HIGH); 
      delay(100);
      digitalWrite(3,LOW);  
      delay(100);
  } else {
      digitalWrite(3,LOW);  // Sent not success, OFF the LED
      delay(600);
  }

}

