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
#include "Animation.h"
#include "Ticker.h"
#include "Twinkle.cpp"
#include "Plasma.cpp"
#include "Snake.cpp"
#include "Life.cpp"
#include "Sprite.cpp"
#include "RainbowColor.cpp"
#include "SerialReceiver.h"

// Teensy 3.1 pinout:
// ====================
//  0 = RX >> EXP8266 TX
//  1 = TX >> ESP8266 RX
//  2 = LED data pin 1
// 14 = LED data pin 2
//  7 = LED data pin 3

// define LED digital pin, color order, chipset and brightness
#define LED_PIN1    2
#define LED_PIN2    14
#define LED_PIN3    7
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

// publish mqtt message to ESP8266 over Serial1
void mqttPublish(String strTopic, String strMessage) {
  Serial1.print(strTopic);
  Serial1.print(":");
  Serial1.println(strMessage);
  Serial1.flush();
}

// handle received serial data (myqtt messages from ESP8266)
void processSerialData(String strTopic, String strMessage)
{
  // handle all mqtt messages
  if (strTopic.startsWith("mumalab")) {
    if (strTopic == "mumalab/fridge/state") {
      state = strMessage.toInt();
    } else if (strTopic == "mumalab/fridge/brightness") {
      brightness = strMessage.toInt();
      brightness = min(255, max(brightness, 0));
      FastLED.setBrightness(brightness);
    } else if (strTopic == "mumalab/fridge/effect") {
      if (strMessage == "twinkle") {
        animation.setEffect(1);
      } else if (strMessage == "plasma") {
        animation.setEffect(2);
      } else if (strMessage == "snake") {
        animation.setEffect(3);
      } else if (strMessage == "life") {
        animation.setEffect(4);
      } else if (strMessage == "sprite") {
        animation.setEffect(5);
      } else if (strMessage == "rainbow") {
        animation.setEffect(6);
      }
      animation.setState(1);
      ticker.setState(0);
    } else if (strTopic == "mumalab/fridge/ticker/color") {
      ticker.setColor(strMessage);
      ticker.setState(1);
      animation.setState(0);
    } else if (strTopic == "mumalab/fridge/ticker/bounce") {
      ticker.setBounce(strMessage.toInt() == 1 ? true : false);
      ticker.setState(1);
      animation.setState(0);
    } else if (strTopic == "mumalab/fridge/ticker/direction") {
      if (strMessage == "left")
        ticker.setDirection(1);
      else if (strMessage == "right")
        ticker.setDirection(2);
      else if (strMessage == "up")
        ticker.setDirection(3);
      else if (strMessage == "down")
        ticker.setDirection(4);
      ticker.setState(1);
      animation.setState(0);
    } else if (strTopic == "mumalab/fridge/ticker/text") {
      ticker.setText(strMessage);
      ticker.setState(1);
      animation.setState(0);
    }
  }
  // print out debug messages
  Serial.print("Received: ");
  Serial.print(strTopic);
  Serial.print(":");
  Serial.println(strMessage);
  Serial.print("state: ");
  Serial.println(state);
  Serial.print("brightness: ");
  Serial.println(brightness);
  Serial.print("effect: ");
  Serial.println(animation.effect());
  Serial.println("");
}

// one frame of animations
void DrawOneFrame()
{
  if (state == 0 || (animation.state() == 0 && ticker.state() == 0)) {
    allOff();
  } else {
    FastLED.setBrightness(brightness);
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

void doTwinkle() {
  Twinkle twinkle(leds[0], leds.Width(), leds.Height(), true, true);
  twinkle.start();
}

void doPlasma() {
  Plasma plasma(leds[0], leds.Width(), leds.Height());
  plasma.start();
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

void doRainbowColor() {
  RainbowColor rainbowColor(leds[0], leds.Width(), leds.Height());
  rainbowColor.start();
}

void doScrollText() {
  if (ScrollingMsg.UpdateText() == -1) {
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
  } else {
    FastLED.show();
  }
  delay(10);
}

// setup demo code
void setup() {
  // set default speed for serial (for debug with serial monitor)
  Serial.begin(9600);

  // set speed of UART to communicate with ESP8266 WiFi module
  Serial.println("initial UART");
  Serial1.begin(115200);

  // initial the serial receiver with ':' separator
  SerialReceiver::initial(':');

  // initial
  state = 1;
  animation.setState(1);
  animation.setEffect(6);
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
}

// execute demo fade code
void loop()
{
  // draw animation on each loop frame
  DrawOneFrame();

  // handle received serial commands
  if (!SerialReceiver::isReady) {
    //delay(10);
    return;
  }
  // handles the received data (command and message)
  processSerialData(SerialReceiver::command, SerialReceiver::payload);
  // reset current comand and be ready to get new from serial
  SerialReceiver::reset();
}

// handle serial receiver events
void serialEvent1() {
  // passin Serial1 and collect all serial data
  SerialReceiver::processSerialEvent(Serial1);
}
