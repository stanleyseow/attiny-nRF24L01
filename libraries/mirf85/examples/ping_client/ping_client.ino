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


const int bufferSize = 6;
byte buffer[] = { 'H', 'e', 'l', 'l', 'o','!' };

#define CE  PB3 //ATTiny
#define CSN PB4 //ATTiny

void setup(){
  Mirf.cePin = CE;
  Mirf.csnPin = CSN;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  byte RADDR[] = {0xf0, 0xf0, 0xf0, 0xf0, 0xe3};
  byte TADDR[] = {0xf0, 0xf0, 0xf0, 0xf0, 0xe3};
 
  Mirf.payload = bufferSize;
  
  Mirf.channel = 0x50;
  Mirf.rfsetup = 0x06; // 1Mbps, Max power
  Mirf.setRADDR(RADDR);  
  Mirf.setTADDR(TADDR);
  Mirf.config();
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
  if (sent) {   // Blink 3 times
     digitalWrite(3,HIGH); // CE pin 2
     delay(100);
     digitalWrite(3,LOW);  // Ce pin 2
     delay(100);
     digitalWrite(3,HIGH); // CE pin 2
     delay(100);
     digitalWrite(3,LOW);  // Ce pin 2
     delay(100);
     digitalWrite(3,HIGH); // CE pin 2
     delay(100);
     digitalWrite(3,LOW);  // Ce pin 2
     delay(100);
  } else {
    delay(600);
  }

}


  
  
  
