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

#ifndef leds_h
#define leds_h

#include "config.h"

void resetFront() {
  fill_solid(ledsFront[0], ledsFront.Size(), CRGB(0,0,0));
}

void resetBack() {
  fill_solid(ledsBack[0], ledsBack.Size(), CRGB(0,0,0));
}

void blitLeds() {
  for (int16_t idx=0; idx<NUMPIXELS; idx++) {
    yield();
    if (ledsFront(idx) != CRGB(0,0,0))
      leds(idx) = ledsFront(idx);
    else
      leds(idx) = ledsBack(idx);
  }
  // This is used in loop for power managed display of LED's.
  show_at_max_brightness_for_power(); 
}

#endif //leds_h
