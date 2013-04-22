attiny-nRF24L01
===============
Arduino attiny support files for nRF24L01 RF modules, ported from Arduino mirf libraries to use USI as attiny does 
not have hardware SPI.
The example should work without modification to the forked RF24 libraries at https://github.com/stanleyseow/RF24
Updated to support TinyDebugSerial for a TX only serial debugging.

Setup
=====

- Copy hardware and libraries folder to where you store your arduino codes (does not need to be the arduino folder) 
- See the examples codes in the libraries/mirf85/examples for all the headers files and details

Defaults used in the codes :-
- Uses CRC16 
- Uses 1Mbps data rate ( Mirf.rfsetup = 0x06 )
- node addressing follows RF24 addressing instead of clie1 or serv1
- Enable dynamic payload for receiver


Pinout
======
attiny85 to nRF24L01
- Pin 1 / Reset
- Pin 2 / D3  / PB3 = nRF3 (CE)
- Pin 3 / D4  / PB4 = nRF4 (CSN)
- Pin 4 / GND / GND = nRF1 (GND)
- Pin 5 / D0  / PB0 = nRF7 (MISO)
- Pin 6 / D1  / PB1 = nRF6 (MOSI)
- Pin 7 / D2  / PB2 = nRF5 (SCK)
- Pin 8 / VCC       = nRF2(VCC/3.3V )

( PBx = AVR ports/pins , Dx  = Arduino pins )

Links
=====
- http://arduino-for-beginners.blogspot.com/2013/04/process-of-getting-attiny85-to-work.html
- http://arduino.cc/forum/index.php/topic,115822.msg1194156.html#msg1194156
- http://www.insidegadgets.com/2012/08/22/using-the-nrf24l01-wireless-module/


Contacts
========
Stanley Seow ( stanleyseow@gmail.com )

