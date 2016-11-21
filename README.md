Refrigerator Lights
===================
Refrigerator Lights is a simple blinkenlights update of the refrigerator of the [MunichMakerLab](https://munichmakerlab.de/) hackerspace.

It drives a small WS2812b LED strip based matrix of 15x10 LEDs behind a diffuse cover; which replaces the old lamp. 
Controllable by MQTT over ESP8266 WiFi module.

[![Refrigerator Lights screenshot](https://cloud.githubusercontent.com/assets/6171589/7560420/513b217e-f7c3-11e4-8fe3-a6d3037b3baf.jpg)](https://www.tumblr.com/video/munichmakerlab/116758165625/700/)

Features
--------
* FastLED library for all the fancy animations
* WeMos d1 mini (ESP8266 devboard) as MCU and WiFi access
* MQTT to remote control the animations and state

Hardware
--------
* WS2812b LED strips (15x10 pixel ~ 50x15 cm)
* 5V 12A power supply
* WeMos d1 mini (ESP8266 devboard)

Wiring ([Fritzing file](/fritzing/esp8266-fridge-lights-wiring.fzz))
------
![wiring diagram](/fritzing/esp8266-fridge-lights-wiring.png "wiring diagram")

Libraries
---------
* [FastLED](https://github.com/FastLED/FastLED)
* [LEDMatrix](https://github.com/Jorgen-VikingGod/LEDMatrix) XY-Matrix class for FastLED
* [PubSubClient](https://github.com/Imroy/pubsubclient) MQTT library by @Imroy.
* [LedEffects](https://bitbucket.org/ratkins/ledeffects/src/?at=default) by ratkins

Requirements
------------
You have to install the Arduino IDE 1.6.5.
* **Arduino** > **Preferences** > "Additional Boards Manager URLs:" and add: **http://arduino.esp8266.com/stable/package_esp8266com_index.json**
* **Arduino** > **Tools** > **Board** > **Boards Manager** > type in **ESP8266** and install the board
* download all libraries and copy folders to Arduino library path.

Upload Sketch
-------------
* Upload ESP8266 Sketch Data: **Tools** > **ESP8266 Sketch Data Upload**
*  ![upload sketch data on ArduinoIDE](/arduino_upload_sketch_data.png "upload sketch data")
* Upload Sketch: **Sketch** > **Upload**

MQTT Topics
-----------
| topic                                  | message                                                               |
| -------------------------------------- | --------------------------------------------------------------------- |
| FridgeLights/13435218/config           | reboot                                                                |
| FridgeLights/13435218/mode             | normal, alert, tetris                                                 |
| FridgeLights/13435218/brightness       | 0 to 255, up, down                                                    |
| FridgeLights/13435218/background       | black, plasma, snake, life, fire, lava, cloud, blackwhite, blackgreen |
| FridgeLights/13435218/color            | **r,g,b**: e.g. **255,0,0** = red                                     |
| FridgeLights/13435218/color/effect     | solid, hue, random                                                    |
| FridgeLights/13435218/tetris           | reset                                                                 |
| FridgeLights/13435218/tetris/move      | left, right, up, down                                                 |
| FridgeLights/13435218/brightness       | 0 to 255                                                              |

**not yet implemented:**

| topic                                  | message                                            |
| -------------------------------------- | -------------------------------------------------- |
| FridgeLights/13435218/ticker/color     | #000000 to #ffffff                                 |
| FridgeLights/13435218/ticker/bounce    | 1 / 0                                              |
| FridgeLights/13435218/ticker/direction | left / right / up / down                           |
| FridgeLights/13435218/ticker/text      | scroll message as text...                          |


TODOs
-----
* bugfix and add the text ticker functionallity
