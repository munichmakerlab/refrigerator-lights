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
 */

#ifndef snake_h
#define snake_h

#include "config.h"
#include "my_mqtt.h"

void resetBack();

static const byte SNAKE_LENGTH = 20;
enum Direction {
  UP, DOWN, LEFT, RIGHT
};
struct Pixel {
  uint8_t x;
  uint8_t y;
};
CRGB colours[SNAKE_LENGTH];
uint8_t initialHue;
Pixel pixels[SNAKE_LENGTH];
Direction direction;

void newDirection() {
  switch (direction) {
    case UP:
    case DOWN:
      direction = random(0, 2) == 1 ? RIGHT : LEFT;
      break;
    case LEFT:
    case RIGHT:
      direction = random(0, 2) == 1 ? DOWN : UP;
    default:
      break;
  }
}

bool shuffleDown() {
  for (byte i = SNAKE_LENGTH - 1; i > 0; i--) {
    processMQTTLoop();
    if (mqttNewMessage)
      return false;
    pixels[i] = pixels[i - 1];
  }
  return true;
}
  
void resetSnake() {
  direction = UP;
  initialHue = 0;
  resetBack();  
}

void snake() {
  if (abs(millis() - LastLoop) >= 50) {
    LastLoop = millis();
    resetBack();
    if (!shuffleDown())
      return;
    if (random(10) > 6) {
      newDirection();
    }
    switch (direction) {
      case UP:
        pixels[0].y = (pixels[0].y + 1) % MATRIX_HEIGHT;
        break;
      case LEFT:
        pixels[0].x = (pixels[0].x + 1) % MATRIX_WIDTH;
        break;
      case DOWN:
        pixels[0].y = pixels[0].y == 0 ? MATRIX_HEIGHT - 1 : pixels[0].y - 1;
        break;
      case RIGHT:
        pixels[0].x = pixels[0].x == 0 ? MATRIX_WIDTH - 1 : pixels[0].x - 1;
        break;
    }
    fill_rainbow(colours, SNAKE_LENGTH, initialHue++);
    for (byte i = 0; i < SNAKE_LENGTH; i++) {
      processMQTTLoop();
      if (mqttNewMessage)
        return;
      ledsBack(pixels[i].x, pixels[i].y) = colours[i] %= (255 - i * (255 / SNAKE_LENGTH));
    }
  }
}

#endif //snake_h
