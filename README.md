attiny-nRF24L01
===============
Arduino attiny support files for nRF24L01 RF modules

Setup
=====
See the examples codes in the mirf85/example for all the headers files

Pinout
======
attiny85 to nRF24L01
Pin 1 / Reset
Pin 2 / D3  / PB3 = nRF3 (CE)
Pin 3 / D4  / PB4 = nRF4 (CSN)
Pin 4 / GND / GND = nRF1 (GND)
Pin 5 / D0  / PB0 = nRF7 (MISO)
Pin 6 / D1  / PB1 = nRF6 (MOSI)
Pin 7 / D2  / PB2 = nRF5 (SCK)
Pin 8 / VCC       = nRF2(VCC/3.3V )

PBx = AVR ports/pins
Dx  = Arduino pins

Links
=====
http://arduino.cc/forum/index.php/topic,115822.msg1194156.html#msg1194156
http://www.insidegadgets.com/2012/08/22/using-the-nrf24l01-wireless-module/

Contacts
========
Stanley Seow ( stanleyseow@gmail.com )

