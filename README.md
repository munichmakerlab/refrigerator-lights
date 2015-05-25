Refrigerator Lights
===================
Refrigerator Lights is a simple blinkenlights update of the refrigerator of the [MunichMakerLab](https://munichmakerlab.de/) hackerspace.

It drives a small WS2812b LED strip based matrix of 15x10 LEDs behind a diffuse cover; which replaces the old lamp. 
Controllable by MQTT over ESP8266 WiFi module.

[![Refrigerator Lights screenshot](https://cloud.githubusercontent.com/assets/6171589/7560420/513b217e-f7c3-11e4-8fe3-a6d3037b3baf.jpg)](https://www.tumblr.com/video/munichmakerlab/116758165625/700/)

Features
--------
* FastLED library for all the fancy animations
* ESP8266 WiFi module for remote access
* MQTT to remote control the animations and state

Hardware
--------
* WS2812b LED strips (15x10 ~ 50x15cm)
* 5V 12A power supply
* Teensy 3.1 microcontroller
* ESP8266 (ESP-12) WiFi board
* [ESP8266-Adapter-Board](http://www.electrodragon.com/product/esp8266-smd-adapter-board) with 3.3V regulator

Wiring
------
![wiring diagram](/wiring/refrigerator-lights-wiring.png "wiring diagram")

Libraries
---------
* [Teensy Libraries](https://github.com/munichmakerlab/refrigerator-lights/tree/master/Teensy#libraries)
* [ESP8266 Libraries](https://github.com/munichmakerlab/refrigerator-lights/tree/master/ESP8266#libraries)

MQTT Topics
-----------
* [MQTT Topics](https://github.com/munichmakerlab/refrigerator-lights/tree/master/ESP8266#mqtt-topics)

TODOs
-----
* bugfix and add the text ticker class
