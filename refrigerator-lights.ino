/******************************************************************************
  The MIT License (MIT)

  Copyright (c) 2015 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
******************************************************************************/

// define, to tell FastLED 3.1 to work as stable version
#define FASTLED_INTERNAL

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>
#include <espduino.h>
#include <mqtt.h>
#include "Animation.h"
#include "Ticker.h"
#include "Twinkle.cpp"
#include "Plasma.cpp"
#include "Snake.cpp"
#include "Life.cpp"
#include "Sprite.cpp"
#include "RainbowColor.cpp"

// Teensy 3.1 pinout:
// ====================
// 0 = RX >> EXP8266 TX
// 1 = TX >> ESP8266 RX
// 2 = ESP8266 CH_PD
// 3 = LED data pin 1
// 4 = LED data pin 2
// 5 = LED data pin 3

// initial ESP8266
ESP esp(&Serial1, &Serial, 2);
MQTT mqtt(&esp);
boolean wifiConnected = false;

// define LED digital pin, color order, chipset and brightness
#define LED_PIN1    3
#define LED_PIN2    4
#define LED_PIN3    5
#define COLOR_ORDER   GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS  170

// helper variables for brightness and mode
int state = 1;
Animation animation;
Ticker ticker;

int brightness = BRIGHTNESS;

// initial matrix layout (to get led strip index by x/y)
#define MATRIX_WIDTH   15
#define MATRIX_HEIGHT  10
#define MATRIX_TYPE  HORIZONTAL_ZIGZAG_MATRIX

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

cLEDText ScrollingMsg;

const unsigned char TxtDemo[] = { EFFECT_SCROLL_LEFT "      LEFT SCROLL "
                  EFFECT_SCROLL_RIGHT "      LLORCS THGIR"
                  EFFECT_SCROLL_DOWN "      SCROLL DOWN       SCROLL DOWN      " EFFECT_FRAME_RATE "\x04" " SCROLL DOWN      " EFFECT_FRAME_RATE "\x00" " "
                  EFFECT_SCROLL_UP "       SCROLL UP         SCROLL UP       " EFFECT_FRAME_RATE "\x04" "  SCROLL UP       " EFFECT_FRAME_RATE "\x00" " "
                  EFFECT_CHAR_UP EFFECT_SCROLL_LEFT "      UP"
                  EFFECT_CHAR_RIGHT "  RIGHT"
                  EFFECT_CHAR_DOWN "  DOWN"
                  EFFECT_CHAR_LEFT "  LEFT"
                  EFFECT_HSV_CV "\x00\xff\xff\x40\xff\xff" EFFECT_CHAR_UP "       HSV_CV 00-40"
                  EFFECT_HSV_CH "\x00\xff\xff\x40\xff\xff" "  HSV_CH 00-40"
                  EFFECT_HSV_AV "\x00\xff\xff\x40\xff\xff" "  HSV_AV 00-40"
                  EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "  HSV_AH 00-FF"
                  "       " EFFECT_HSV "\x00\xff\xff" "R" EFFECT_HSV "\x20\xff\xff" "A" EFFECT_HSV "\x40\xff\xff" "I" EFFECT_HSV "\x60\xff\xff" "N" EFFECT_HSV "\xe0\xff\xff" "B" EFFECT_HSV "\xc0\xff\xff" "O"
                  EFFECT_HSV "\xa0\xff\xff" "W" EFFECT_HSV "\x80\xff\xff" "S " EFFECT_DELAY_FRAMES "\x00\x96" EFFECT_RGB "\xff\xff\xff" };

// setup demo code
void setup() {
  // set default speed for serial (for debug with serial monitor)
  Serial.begin(19200);  
  // set speed of UART to communicate with ESP8266 WiFi module
  Serial1.begin(19200);
  Serial.println("enable ESP");
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());
  Serial.println("WiFi: ready");

  // initial
  state = 1;
  animation.setState(1);
  animation.setEffect(1);
  ticker.setState(0);

  brightness = 170;
  
  // initial LEDs
  FastLED.addLeds<CHIPSET, LED_PIN1, COLOR_ORDER>(leds[0], 0, 60).setCorrection(TypicalSMD5050);
  FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(leds[0], 60, 60).setCorrection(TypicalSMD5050);
  FastLED.addLeds<CHIPSET, LED_PIN3, COLOR_ORDER>(leds[0], 120, 30).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  
  ScrollingMsg.SetFont(MatriseFontData);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
  ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);
  
  Serial.println("Matrix: initialed");
  
  Serial.println("WiFi: setup mqtt client");
  // please replace "MQTT-USER" with your mqtt user name and "MQTT-PWD" with your password
  if(!mqtt.begin("FRIDGE", "MQTT-USER", "MQTT-PWD", 120, 1)) {
    Serial.println("WiFi: fail to setup mqtt");
    //while(1);
  }

  Serial.println("WiFi: setup mqtt lwt");
  mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

  // setup mqtt events
  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.disconnectedCb.attach(&mqttDisconnected);
  mqtt.publishedCb.attach(&mqttPublished);
  mqtt.dataCb.attach(&mqttData);

  // setup wifi
  Serial.println("WiFi: setup wifi");
  esp.wifiCb.attach(&wifiCb);
  
  // please replace "WLAN-SSID" with your WLAN SSID and "WLAN-PWD" with your WLAN password
  esp.wifiConnect("WLAN-SSID", "WLAN-PWD");

  Serial.println("WiFi: system started");
}

// execute demo fade code
void loop()
{
  esp.process();
  
  if(!wifiConnected) {
    state = 1;
    animation.setState(1);
    animation.setEffect(1);
    ticker.setState(0);
  }
  // draw animation on each loop frame
  DrawOneFrame();
}

// WiFi connection changed
void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      Serial.println("WiFi: WIFI CONNECTED");
      // please replace "mqtt.yourdomain.net" with your mqtt broker domain or server ip
      mqtt.connect("mqtt.yourdomain.net", 1883, false);
      wifiConnected = true;
      //or mqtt.connect("host", 1883); /*without security ssl*/
    } else {
      Serial.println("WiFi: WIFI DISCONNECTED");
      wifiConnected = false;
      mqtt.disconnect();
    }
  } else {
    Serial.println("WiFi: wifiCb != 1");
  }
}

// myqtt connected
void mqttConnected(void* response)
{
  Serial.println("WiFi: Connected");
  mqtt.subscribe("mumalab/fridge/state");
  mqtt.subscribe("mumalab/fridge/brightness");
  mqtt.subscribe("mumalab/fridge/mode/animation/state");
  mqtt.subscribe("mumalab/fridge/mode/animation/effect");
  mqtt.subscribe("mumalab/fridge/mode/ticker/state");
  mqtt.subscribe("mumalab/fridge/mode/ticker/color");
  mqtt.subscribe("mumalab/fridge/mode/ticker/bounce");
  mqtt.subscribe("mumalab/fridge/mode/ticker/direction");
  mqtt.subscribe("mumalab/fridge/mode/ticker/text");

  mqtt.publish("mumalab/fridge/connected", "connected");
}

// mqtt disconnected
void mqttDisconnected(void* response)
{

}

// myqtt data received
void mqttData(void* response)
{
  RESPONSE res(response);

  Serial.print("WiFi: Received: topic=");
  String topic = res.popString();
  Serial.println(topic);

  Serial.print("WiFi: data=");
  String data = res.popString();
  Serial.println(data);
  
  if (topic == "mumalab/fridge/state") {
    state = data == "on" ? 1 : 0;
    Serial.print("mumalab/fridge/state: ");
    Serial.println(state == 1 ? "on" : "off");
  } else if (topic == "mumalab/fridge/brightness") {
    brightness = data.toInt();
    brightness = min(255, max(brightness, 0));
    FastLED.setBrightness(brightness);
    Serial.print("mumalab/fridge/brightness: ");
    Serial.println(data);
  } else if (topic == "mumalab/fridge/mode/animation/state") {
    animation.setState(data == "on" ? 1 : 0);
    Serial.print("mumalab/fridge/mode/animation/state: ");
    Serial.println(animation.state() == 1 ? "on" : "off");
    ticker.setState(0);
    mqtt.publish("mumalab/fridge/mode/ticker/state", "off");
    if (animation.state() == ticker.state()) {
      state = 0;
      mqtt.publish("mumalab/fridge/state", "off");
    }
  } else if (topic == "mumalab/fridge/mode/animation/effect") {
    String effectName = data;
    if (effectName == "twinkle") {
      animation.setEffect(1);
    } else if (effectName == "plasma") {
      animation.setEffect(2);
    } else if (effectName == "snake") {
      animation.setEffect(3);
    } else if (effectName == "life") {
      animation.setEffect(4);
    } else if (effectName == "sprite") {
      animation.setEffect(5);
    } else if (effectName == "rainbow") { 
      animation.setEffect(6);
    }
    Serial.print("mumalab/fridge/mode/animation/effect: ");
    Serial.println(effectName);
 } else if (topic == "mumalab/fridge/mode/ticker/state") {
    ticker.setState(data == "on" ? 1 : 0);
    Serial.print("mumalab/fridge/mode/ticker/state: ");
    Serial.println(ticker.state() == 1 ? "on" : "off");
    animation.setState(0);
    mqtt.publish("mumalab/fridge/mode/animation/state", "off");
    if (ticker.state() == animation.state()) {
      state = 0;
      mqtt.publish("mumalab/fridge/state", "off");
    }
  } else if (topic == "mumalab/fridge/mode/ticker/color") {
    ticker.setColor(data);
    Serial.print("mumalab/fridge/mode/ticker/color: ");
    Serial.println(data);
  } else if (topic == "mumalab/fridge/mode/ticker/bounce") {
    ticker.setBounce(data.toInt() == 1 ? true : false);
    Serial.print("mumalab/fridge/mode/ticker/bounce: ");
    Serial.println(ticker.bounce() ? "true" : "false");
  } else if (topic == "mumalab/fridge/mode/ticker/direction") {
    if (data == "left")
      ticker.setDirection(1);
    else if (data == "right") 
      ticker.setDirection(2);
    else if (data == "up") 
      ticker.setDirection(3);
    else if (data == "down") 
      ticker.setDirection(4);
    Serial.print("mumalab/fridge/mode/ticker/direction: ");
    Serial.println(data);
  } else if (topic == "mumalab/fridge/mode/ticker/text") {
    ticker.setText(data);
    Serial.print("mumalab/fridge/mode/ticker/text: ");
    Serial.println(data);    
  }
}

// mqtt data sent
void mqttPublished(void* response)
{

}

// one frame of animations
void DrawOneFrame()
{
  if (state == 0 || (animation.state() == 0 && ticker.state() == 0)) {
    allOff();
  } else {
    if (animation.state() == 1) {
      switch (animation.effect()) {
        default: 
        case 1:  doTwinkle();       break;
        case 2:  doPlasma();        break;
        case 3:  doSnake();         break;
        case 4:  doLife();          break;
        case 5:  doSprite();        break;
        case 6:  doRainbowColor();  break;
      }
    } else if (ticker.state() == 1) {
      doScrollText();
    }
  }
}

// all LEDs black
void allOff() {
  fill_solid(leds[0], leds.Size(), CRGB::Black);
  FastLED.setBrightness(0);
  FastLED.show();
}

void doDeadChannel() {
  DeadChannel deadChannel(leds[0], leds.Width(), leds.Height());
  deadChannel.start();
}

void doPlasma() {
  Plasma plasma(leds[0], leds.Width(), leds.Height());
  plasma.start();
}

void doTwinkle() {
  Twinkle twinkle(leds[0], leds.Width(), leds.Height(), true, true);
  twinkle.start();
}

void doSnake() {
  Snake snake(leds[0], leds.Width(), leds.Height());
  snake.start();
}

void doLife() {
  Life life(leds[0], leds.Width(), leds.Height(), 56);
  life.start();
}

void doSprite() {
  Sprite sprite(leds[0], leds.Width(), leds.Height());
  sprite.start();
}

void doScrollText() {
  if (ScrollingMsg.UpdateText() == -1) {
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
  } else {
    FastLED.show();
  }
  delay(10);
}

void doRainbowColor() {
  RainbowColor rainbowColor(leds[0], leds.Width(), leds.Height());
  rainbowColor.start();
}

