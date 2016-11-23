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
 * 
 * Effect implementation for the GauntletII project.
 * Created by Robert Atkins on 26/09/13.
 * Copyright (c) 2013 Robert Atkins. All rights reserved.
 */

#ifndef life_h
#define life_h

#include "config.h"
#include "my_mqtt.h"

void resetBack();

const int deltaLen = 36;

int gol_framerate;
int gol_brightness;
int gol_time;
int gol_timeMax;
uint8_t density;
uint8_t hue;
uint16_t delta[deltaLen];

void seed(uint8_t hue) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (random(255) < density) {
      ledsBack(i) = CHSV(hue, 255, 255);
    }
  }
}

bool inXRange(int x) {
  return x >= 0 && x < MATRIX_WIDTH;
}

bool inYRange(int y) {
  return y >= 0 && y < MATRIX_HEIGHT;
}

bool alive(int x, int y) {
  return inXRange(x) && inYRange(y) && ledsBack(x, y);
}

int deltaIndex(int x, int y) {
  return y * 2 + (x < 16 ? 0 : 1);
}

// xxxx xxxx xxxx xxxx  xxxx xxxx xxxx xxxx
// xxxx xxxx xxxx xxxx  xxxx xxxx xxxx xxxx
void setChanged(int x, int y) {
  delta[deltaIndex(x, y)] |= 1 << (x < 16 ? x : x - 16);
}

void updateWithChanges(uint8_t time) {
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      if (delta[deltaIndex(x, y)] & (1 << (x < 16 ? x : x - 16))) {
        processMQTTLoop();
        if (mqttNewMessage)
          return;
        if (ledsBack(x, y)) {
          ledsBack(x, y) = CRGB::Black;
        } else {
          ledsBack(x, y) = CHSV(time, 255, 255);
        }
      }
    }
  }
}

/*
x - 1, y + 1|x = x, y + 1|x + 1, y + 1
x - 1, y = y|    x   |x + 1, y = y
x - 1, y - 1|x = x, y - 1|x + 1, y - 1
*/
int numNeighbours(int x, int y) {
  int numNeighbours = 0;
  numNeighbours += alive(x - 1, y + 1) ? 1 : 0;
  numNeighbours += alive(x, y + 1) ? 1 : 0;
  numNeighbours += alive(x + 1, y + 1) ? 1 : 0;
  numNeighbours += alive(x - 1, y) ? 1 : 0;
  numNeighbours += alive(x + 1, y) ? 1 : 0;
  numNeighbours += alive(x - 1, y - 1) ? 1 : 0;
  numNeighbours += alive(x, y - 1) ? 1 : 0;
  numNeighbours += alive(x + 1, y - 1) ? 1 : 0;
  return numNeighbours;
}

void resetLife() {
  density = 56;
  gol_brightness = 0;
  gol_framerate = 100;
  gol_time = 0;
  gol_timeMax = random(96, 156);
  resetBack();
  uint8_t hue = random(256);
  seed(hue);
}

void fadeout() {
  if (gol_brightness < 256) {
    for (int i = 0; i < NUMPIXELS; i++) {
      processMQTTLoop();
      if (mqttNewMessage)
        return;
      ledsBack(i)--;
    }
    gol_brightness++;
  } else {
    resetLife();
  }
}

void life() {
  if (abs(millis() - LastLoop) >= gol_framerate) {
    LastLoop = millis();
    if (gol_time < gol_timeMax) {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        for (int y = 0; y < MATRIX_HEIGHT; y++) {
          processMQTTLoop();
          if (mqttNewMessage)
            return;
          int neighbours = numNeighbours(x, y);
          if (ledsBack(x, y)) {
            if (neighbours < 2 || neighbours > 3) {
              setChanged(x, y);
            }
          } else {
            if (neighbours == 3) {
              setChanged(x, y);
            }
          }
        }
      }
      updateWithChanges(hue++);
      for (int i = 0; i < deltaLen; i++) {
        delta[i] = 0;
      }
      gol_time++;
    } else {
      gol_framerate = 25;
      fadeout();
    }
  }
}

#endif //life_h
