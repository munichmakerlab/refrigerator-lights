ESP8266 sources
===============
The ESP8266 module is programmed directly with Arduino IDE.
All the network and MQTT implementation is now on the ESP8266 and not on Teensy.
There is also a simple passthrough of Serial stream, to handle all the MQTT messages by Teensy.

Requirements
------------
You have to install the Arduino IDE 1.6.4.
* **Arduino** > **Preferences** > "Additional Boards Manager URLs:" and add: **http://arduino.esp8266.com/package_esp8266com_index.json**
* **Arduino** > **Tools** > **Board** > **Boards Manager** > type in **ESP8266** and install the board
* download PubSubClient (see [Libraries](https://github.com/munichmakerlab/refrigerator-lights/tree/master/ESP8266#libraries)) and copy folder to Arduino library path

Hardware
--------
* ESP8266 (ESP-12) WiFi board
* [ESP8266-Adapter-Board](http://www.electrodragon.com/product/esp8266-smd-adapter-board) with 3.3V regulator

Libraries
---------
* [PubSubClient](https://github.com/Imroy/pubsubclient) MQTT library by @Imroy.

MQTT Topics
-----------
| topic                           | message                                            |
| ------------------------------- | -------------------------------------------------- |
| mumalab/fridge/uptime           | String(millis())                                   |
| mumalab/fridge/state            | 1 / 0                                              |
| mumalab/fridge/brightness       | 0 to 255                                           |
| mumalab/fridge/effect           | twinkle / plasma / snake / life / sprite / rainbow |
| mumalab/fridge/ticker/color     | #000000 to #ffffff                                 |
| mumalab/fridge/ticker/bounce    | 1 / 0                                              |
| mumalab/fridge/ticker/direction | left / right / up / down                           |
| mumalab/fridge/ticker/text      | scroll message as text...                          |

TODOs
-----
* bugfix and add the text ticker class
