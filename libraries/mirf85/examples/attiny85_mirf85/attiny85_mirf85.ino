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

 Added TinyDebugSerial to the codes for TX only serial debugging
 https://code.google.com/p/arduino-tiny/

*/


#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <SPI85.h>
#include <Mirf85.h>
#include <nRF24L0185.h>
#include <MirfHardwareSpiDriver85.h>
#include <TinyDebugSerial.h>
TinyDebugSerial mySerial = TinyDebugSerial(); // PB3 on tiny85 = TX pin


// ATtiny25/45/85 Pin map
//                                 +-\/-+
//                Reset/Ain0 (D 5) PB5  1|o   |8  Vcc
//  nRF24L01 CE, Pin3 - Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1 - nRF24L01 SCK, pin5
// nRF24L01 CSN, Pin4 - Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1 - nRF24L01 MOSI, pin7
//                                 GND  4|    |5  PB0 (D 0) pwm0 - nRF24L01 MISO, pin6
// Pinout
// CE   - Pin 2 / D3 / nRF3
// CSN  - Pin 3 / D4 / nRF4
// GND  - Pin 4
// MISO - Pin 5 / D0 / nRF7
// MOSI - Pin 6 / D1 / nRF6
// SCK  - Pin 7 / D2 / nRF5
// Vcc  - Pin 8 ( 3.3V )

int bufferSize = 0;
char buffer[32] = "";
uint16_t counter = 0; 
uint8_t nodeID = 0;

#define CE  PB3 //ATTiny
#define CSN PB4 //ATTiny

void setup(){
  
  mySerial.begin( 9600 );    // for tiny_debug_serial 
  
  Mirf.cePin = CE;
  Mirf.csnPin = CSN;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // This address is compatible with my example of rpi-hub or nRF24_Arduino_as_hub
  // at repo https://github.com/stanleyseow/RF24/examples/ 
  
  byte RADDR[] = {0xe2, 0xf0, 0xf0, 0xf0, 0xf0};
  byte TADDR[] = {0xe3, 0xf0, 0xf0, 0xf0, 0xf0};
 
  // Get nodeID from TXADDR and mask the first byte (0xff)
  nodeID = *TADDR & 0xff; 
  
  Mirf.channel = 0x4c; // Same as rpi-hub and sendto_hub
  Mirf.rfsetup = 0x06; // 1Mbps, Max power
  Mirf.setRADDR(RADDR);  
  Mirf.setTADDR(TADDR);
  Mirf.config();
  
  // Enable dynamic payload 
  Mirf.configRegister( FEATURE, 1<<EN_DPL ); 
  Mirf.configRegister( DYNPD, 1<<DPL_P0 | 1<<DPL_P1 | 1<<DPL_P2 | 1<<DPL_P3 | 1<<DPL_P4 | 1<<DPL_P5 ); 
  
  
  delay(100);
  
  // Print out register readinds for important settings
  uint8_t rf_ch, rf_setup = 0;
  byte tx_addr[5];
  byte rx_addr[5];
  
  Mirf.readRegister(RF_CH, &rf_ch,sizeof(rf_ch));
  Mirf.readRegister(RF_SETUP, &rf_setup, sizeof(rf_setup));
  Mirf.readRegister(TX_ADDR, tx_addr, sizeof(tx_addr));
  Mirf.readRegister(RX_ADDR_P1, rx_addr, sizeof(rx_addr));
  
   mySerial.println();
  
  mySerial.print("RF_CH :");
  mySerial.println(rf_ch,HEX);  
  
  mySerial.print("RF_SETUP :");
  mySerial.println(rf_setup,BIN);  
  
  mySerial.print("TX_ADDR :");
  for ( int i=0;i<5;i++ ) {  // Loop 5 times, print in HEX
  mySerial.print( tx_addr[i], HEX);
  }
  mySerial.println();
  
  mySerial.print("RX_ADDR :");
  for ( int i=0;i<5;i++ ) {  // Loop 5 times, print in HEX
  mySerial.print( rx_addr[i], HEX);
  }
  mySerial.println();
    
  delay(2000);      // For serial debug to read the init config output
  
}

void loop(){
  
  uint8_t sent = false;
  uint8_t sensor1;

  // Read from sensor at pin 4 (CSN)
  pinMode(4, INPUT);
  delay(10);
  sensor1 = digitalRead(4);
  pinMode(4, OUTPUT);

  sprintf(buffer,"%2X,%04d,%06d",nodeID,sensor1,counter++);
  Mirf.payload = strlen(buffer);
  
  Mirf.send((byte *) buffer);
  while( Mirf.isSending() )
  {
    delay(1);
    sent = true; // Sent success
  }
  
    delay(500);  
    
    if (sent) {   // Blink CE (pin 2), 3 times, unused when not in TX/RX mode 
        mySerial.print("Sent :");
        mySerial.println(buffer);
        /*
        digitalWrite(3,HIGH);  
        delay(50);
        digitalWrite(3,LOW);  
        delay(50);
        digitalWrite(3,HIGH); 
        delay(50);
        digitalWrite(3,LOW);  
        delay(50);
        digitalWrite(3,HIGH); 
        delay(50);
        digitalWrite(3,LOW);  
        delay(50);
        */
    } 
   
    delay(500);
    
} // End loop()

