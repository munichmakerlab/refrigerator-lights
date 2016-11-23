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

#ifndef front_effects_h
#define front_effects_h

#include "my_mqtt.h"

void resetBack();

CRGBPalette16 myPal = HeatColors_p;

void alert() {
  EVERY_N_MILLISECONDS(50) {
    resetFront();
    processMQTTLoop();
    if (mqttNewMessage)
      return;
    CRGB alertCol = ColorFromPalette(myPal, random8(0,255));
    ledsFront.DrawRectangle(1,1, 13,8, alertCol);
    ledsFront.DrawRectangle(3,3, 11,6, alertCol);
  }   
}

#endif //front_effects_h
