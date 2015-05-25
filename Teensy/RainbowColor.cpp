//
//  RainbowColor.cpp
//  based on animation code of Mark Kriegsman
//
//  Created by Juergen Skrotzky on 05/05/2015.
//  Copyright (c) 2015 Juergen Skrotzky alias JorgenVikingGod. All rights reserved.
//

#include "Effect.h"

class RainbowColor : public Effect {
    
public:
    
    RainbowColor(CRGB *leds, int width, int height) : Effect(leds, width, height) {
    }
    
    void start() {
        uint32_t ms = millis();
        int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * ( 350 / width ));
        int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * ( 310 / height ));
        byte startHue8 = ms / 65536;
        byte lineStartHue = startHue8;
        int8_t yHueDelta8 = yHueDelta32 / 32768;
        int8_t xHueDelta8 = xHueDelta32 / 32768;
        for( byte y = 0; y < height; y++) {
            lineStartHue += yHueDelta8;
            byte pixelHue = lineStartHue;      
            for( byte x = 0; x < width; x++) {
                pixelHue += xHueDelta8;
                pixel(x, y) = CHSV(pixelHue, 255, 255);
            }
        }
        FastLED.show();
    }
};


