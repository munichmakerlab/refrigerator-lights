Teensy sources
===================
Teensy 3.1 board has the complete implementation of the LED animations.
The Communication with the ESP8266 module is done by UART (Serial1) by simple passthrough serial data.

Hardware
--------
* WS2812b LED strips (15x10 ~ 50x15cm)
* 5V 12A power supply
* Teensy 3.1 microcontroller

Libraries
---------
* [FastLED](https://github.com/FastLED/FastLED) (LED light controller by FastLED library)
* [RGBLEDS](https://github.com/AaronLiddiment/RGBLEDS) (LEDMatrix & LEDText)
* [LedEffects by ratkins](https://bitbucket.org/ratkins/ledeffects/src/?at=default)

MQTT Topics
-----------
* [MQTT Topics](https://github.com/munichmakerlab/refrigerator-lights/tree/master/ESP8266#mqtt-topics) see README of ESP8266
