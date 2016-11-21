/*
 * refrigerator-lights: https://github.com/munichmakerlab/refrigerator-lights
 * Copyright (C) 2016 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)
 *
 * This sketch lights up a 15x10 WS2812b RGB LEDs.
 * The lights are fully controllable over WiFi by MQTT.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <TimeLib.h>
#include <math.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>          //https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>    //https://github.com/me-no-dev/ESPAsyncWebServer
#include <DNSServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <WiFiClient.h>   
#include <PubSubClient.h>
#include <MQTT.h>
#include <WiFiUdp.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>              //https://github.com/FastLED/FastLED
#include <LEDMatrix.h>            //https://github.com/Jorgen-VikingGod/LEDMatrix
#include <LEDSprites.h>           //https://github.com/AaronLiddiment/LEDSprites
#include <LEDText.h>              //https://github.com/AaronLiddiment/LEDText
#include <FontMatrise.h>
#include <QueueArray.h>           //http://playground.arduino.cc/Code/QueueArray
#include "config.h"
#include "my_spiffs_editor.h"
#include "my_mqtt.h"
#include "leds.h"
#include "tetris.h"
#include "noise.h"
#include "snake.h"
#include "life.h"
#include "front_effects.h"

void mqttMessageReceived(const MQTT::Publish& pub) {
  // handle message arrived
  String mqttTopic = pub.topic();
  String mqttPayload = pub.payload_string();
  mqttMessages.push(sMqttMessage(mqttTopic,mqttPayload));
  DEBUG_PRINTLN("receive MQTT: topic='" + mqttTopic + "', message='" + mqttPayload + "'");
  mqttNewMessage = true;
  // OTA: "FridgeLights/<CHIP-ID>/ota", bin file to flash
  if (mqttTopic == String(strTopicPrefixChipID + "ota")) {
    uint32_t startTime = millis();
    uint32_t size = pub.payload_len();
    if (size == 0)
      return;
    DEBUG_PRINT("Receiving OTA of ");
    DEBUG_PRINT(size);
    DEBUG_PRINTLN(" bytes...");
    Serial.setDebugOutput(true);
    if (ESP.updateSketch(*pub.payload_stream(), size, true, false)) {
      pub.payload_stream()->stop();
      DEBUG_PRINTLN("Clearing retained message.");
      mqttClient.publish(MQTT::Publish(pub.topic(), "").set_retain());
      mqttClient.disconnect();
      DEBUG_PRINT("Update Success: ");
      DEBUG_PRINT(millis() - startTime);
      DEBUG_PRINTLN("\nRebooting...\n");
      ESP.restart();
      delay(10000);
    }
  }
}

void handleMQTTMessages() {
  // reset flag
  if (mqttMessages.isEmpty())
    return;
  mqttNewMessage = false;
  // get one message from queue
  sMqttMessage newMqttMessage = mqttMessages.pop();
  String mqttTopic   = newMqttMessage.topic;
  String mqttPayload = newMqttMessage.payload;

  // config: "FridgeLights/<CHIP-ID>/config", "reboot"
  if (mqttTopic == String(strTopicPrefixChipID + "config")) {
    if (mqttPayload == "reboot") {
      DEBUG_PRINTLN("force reboot");
      ESP.restart();
      return;
    }
  }
  // mqtt/host: "FridgeLights/<CHIP-ID>/mqtt/host", "mqtt.your-host.com" (mqtt host name or ip)
  if (mqttTopic == String(strTopicPrefixChipID + "mqtt/host")) {
    strcpy(mqtt_host, mqttPayload.c_str());
    // use configured broker hostname/port
    mqttClient.set_server(mqtt_host, String(mqtt_port).toInt());
    DEBUG_PRINT("mqtt host ");
    DEBUG_PRINTLN(mqtt_host);
    return;
  }
  // mqtt/port: "FridgeLights/<CHIP-ID>/mqtt/port", "1883" (mqtt broker port)
  if (mqttTopic == String(strTopicPrefixChipID + "mqtt/port")) {
    strcpy(mqtt_port, mqttPayload.c_str());
    // use configured broker hostname/port
    mqttClient.set_server(mqtt_host, String(mqtt_port).toInt());
    DEBUG_PRINT("mqtt port ");
    DEBUG_PRINTLN(mqtt_port);
    return;
  }
  // mqtt/user: "FridgeLights/<CHIP-ID>/mqtt/user", "user" (mqtt broker user name)
  if (mqttTopic == String(strTopicPrefixChipID + "mqtt/user")) {
    strcpy(mqtt_user, mqttPayload.c_str());
    DEBUG_PRINT("mqtt user ");
    DEBUG_PRINTLN(mqtt_user);
    return;
  }
  // mqtt/pass: "FridgeLights/<CHIP-ID>/mqtt/pass", "pass" (mqtt broker user password)
  if (mqttTopic == String(strTopicPrefixChipID + "mqtt/pass")) {
    strcpy(mqtt_pass, mqttPayload.c_str());
    DEBUG_PRINT("mqtt password ");
    DEBUG_PRINTLN(mqtt_pass);
    return;
  }
  // mqtt/id: "FridgeLights/<CHIP-ID>/mqtt/id", "WordClock" (mqtt broker client id)
  if (mqttTopic == String(strTopicPrefixChipID + "mqtt/id")) {
    strcpy(mqtt_client_id, mqttPayload.c_str());
    DEBUG_PRINT("mqtt client id ");
    DEBUG_PRINTLN(mqtt_client_id);
    return;
  }
  // brightness: "FridgeLights/<CHIP-ID>/brightness", "0-255"
  if (mqttTopic == String(strTopicPrefixChipID + "brightness")) {
    if (mqttPayload == "up") {
      brightness += 50;
      if (brightness > 255)
        brightness = 255;
    } else if (mqttPayload == "down") {
      brightness -= 50;
      if (brightness < 0)
        brightness = 0;
    } else {
      brightness = mqttPayload.toInt();
      brightness = std::max(0, std::min(255, brightness));
    }
    DEBUG_PRINT("brightness ");
    DEBUG_PRINTLN(brightness);
    FastLED.setBrightness(brightness);
    show_at_max_brightness_for_power();
    return;
  }
  // mode: "FridgeLights/<CHIP-ID>/mode", "normal" (normal|tetris|ticker)
  if (mqttTopic == String(strTopicPrefixChipID + "mode")) {
    effectMode = mqttPayload != "" ? mqttPayload : "normal";
    resetFront();
    return;
  }
  // color: "FridgeLights/<CHIP-ID>/color", "255,0,0" (red,green,blue)
  if (mqttTopic == String(strTopicPrefixChipID + "color")) {
    LedR = getStringPartByNr(mqttPayload, ',', 0);
    LedG = getStringPartByNr(mqttPayload, ',', 1);
    LedB = getStringPartByNr(mqttPayload, ',', 2);
    return;
  }
  // color/effect: "FridgeLights/<CHIP-ID>/color/effect", "hue" (solid|hue|random)
  if (mqttTopic == String(strTopicPrefixChipID + "color/effect")) {
    colorEffect = mqttPayload != "" ? mqttPayload : "solid";
    return;
  }
  // background: "FridgeLights/<CHIP-ID>/background", "plasma" (black|plasma|lava|fire|cloud|blackwhite|blackgreen)
  if (mqttTopic == String(strTopicPrefixChipID + "background")) {
    background = mqttPayload != "" ? mqttPayload : "black";
    if (background == "snake") {
      resetSnake();
    } else if (background == "life") {
      resetLife();
    }
    return;
  }
  // tetris: "WordClock<CHIP-ID>/tetris", "reset" (reset|...)
  if (mqttTopic == String(strTopicPrefixChipID + "tetris")) {
    if (mqttPayload == "reset") {
      AttractMode = false;
      memset(PlayfieldData, 0, sizeof(PlayfieldData));
      memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
      Sprites.RemoveSprite(&CurrentBlock);
      LastScore = 0;
      TotalLines = 0;
      DropDelay = INITIAL_DROP_FRAMES;
      CurrentBlock.SetXChange(-1);
      NextBlock = true;
    }
    return;
  }
  // tetris/move: "FridgeLights/<CHIP-ID>/tetris/move", "rotate" (rotate|left|right|down)
  if (mqttTopic == String(strTopicPrefixChipID + "tetris/move")) {
    JSRotate.SetState(HIGH);
    JSLeft.SetState(HIGH);
    JSRight.SetState(HIGH);
    JSDown.SetState(HIGH);
    if (mqttPayload == "left") {
      JSLeft.SetState(LOW);
    } else if (mqttPayload == "right") {
      JSRight.SetState(LOW);
    } else if (mqttPayload == "down") {
      JSDown.SetState(LOW);
    } else {
      JSRotate.SetState(LOW);
    }
    return;
  }
}

void setup() {
  delay(500);
  // initial serial port
  if(_debug)
    Serial.begin(9600);

  SPIFFS.begin();

  // define topics and chip id
  strChipID = String(ESP.getChipId());
  strClientID = String(mqtt_client_id);
  strTopicPrefix = strClientID + "/";
  strTopicPrefixChipID = strTopicPrefix + strChipID + "/";

  // set hostname
  strHostname = strClientID + "-" + strChipID;
  WiFi.hostname(strHostname);
  // print log
  DEBUG_PRINTLN("");
  DEBUG_PRINT(strClientID);
  DEBUG_PRINT("(");
  DEBUG_PRINT(strChipID);
  DEBUG_PRINTLN(")");
  DEBUG_PRINT("Hostname: ");
  DEBUG_PRINTLN(strHostname);
  DEBUG_PRINTLN(__TIMESTAMP__);
  DEBUG_PRINT("Sketch size: ");
  DEBUG_PRINTLN(ESP.getSketchSize());
  DEBUG_PRINT("Free size: ");
  DEBUG_PRINTLN(ESP.getFreeSketchSpace());

  randomSeed(analogRead(0));

  DEBUG_PRINTLN("initial FastLED");
  // initial LEDs
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(brightness);
  FastLED.setDither(brightness < 255);
  FastLED.clear(true);

  // This is defined in setup and used by the power management functionality and is currently set at 5V, 10A = 10000mA.
  set_max_power_in_volts_and_milliamps(5, 1000);

  resetFront();
  resetBack();
  blitLeds();

  // bind callback function to handle incomming mqtt messages
  mqttClient.set_callback(mqttMessageReceived);

  // connect wifi
  DEBUG_PRINTLN("");
  DEBUG_PRINT("Connect to WiFi");
  if (ssid1 && pass1) WiFiMulti.addAP(ssid1, pass1);
  if (ssid2 && pass2) WiFiMulti.addAP(ssid2, pass2);
  if (ssid3 && pass3) WiFiMulti.addAP(ssid3, pass3);
  uint8_t hue = 0;
  while(WiFiMulti.run() != WL_CONNECTED) {
    hue+=20;
    fill_rainbow(leds[0], NUMPIXELS, hue, NUMPIXELS);
    FastLED.show(); 
    delay(50);
    DEBUG_PRINT(".");
  }
  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("WiFi connected");
  DEBUG_PRINTLN("IP address: ");
  IPAddress local = WiFi.localIP();
  strIPAddr = String(local[0]) + "." + String(local[1]) + "." + String(local[2]) + "." + String(local[3]);
  DEBUG_PRINTLN(strIPAddr);

  tetrisSetup();
  setupWebserver();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED || mqttClient.connected() != 1) {
    checkConnect();
  }
  mqttClient.loop();
  handleMQTTMessages();

  if (effectMode == "tetris") {
    tetrisLoop();
    // skip loop after one tetris loop
    return;
  }

  yield();
  
  processMQTTLoop();
  if (mqttNewMessage)
    return;
  if (background == "black") {
    resetBack();
  } else if (background == "plasma") {
    // reset of background layer (each 20ms)
    plasma();
  } else if (background == "fire") {
    resetBack();
    fireNoise();
  } else if (background == "lava") {
    resetBack();
    lavaNoise();
  } else if (background == "cloud") {
    resetBack();
    cloudNoise();
  } else if (background == "blackwhite") {
    resetBack();
    blackAndWhiteNoise();
  } else if (background == "blackgreen") {
    resetBack();
    blackAndGreenNoise();
  } else if (background == "snake") {
    snake();
  } else if (background == "life") {
    life();
  }

  processMQTTLoop();
  if (mqttNewMessage)
    return;

  // show front effects
  if (effectMode == "alert") {
    alert();    
  }
  blitLeds();
}

