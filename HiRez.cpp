//
//  HiRez.cpp
//  GauntletII
//
//  Created by Robert Atkins on 26/09/13.
//  Copyright (c) 2013 Robert Atkins. All rights reserved.
//

#include "Effect.h"

class HiRez : public Effect {
    
private:
    
    uint16_t hiWidth, hiHeight;
    uint8_t hue;
    
public:
    
    HiRez(CRGB *leds, int width, int height): Effect(leds, width, height),
        hiWidth(256 * width),
        hiHeight(256 * height),
        hue(0)
    {
    }
        
    void start() {
        lo();
        hi();
    }
    
    void lo() {
        for (int x = 0; x < width; x++) {
            pixel(x - 1, 0) = CHSV(hue, 255, 255);
            pixel(x, 0) = CHSV(hue, 255, 255);
            pixel(x + 1, 0) = CHSV(hue, 255, 255);
            LEDS.show();
            delay(64);
            pixel(x - 1, 0) = CRGB::Black;
            pixel(x, 0) = CRGB::Black;
            pixel(x + 1, 0) = CRGB::Black;
        }
    }
    //
    // x x x x | x x x x | x x x x | x x x x
    
    void hi() {
        for (uint16_t x = 1; x < hiWidth; x+= 32) {
            uint16_t actual = x >> 8;
            
            pixel(actual - 1, 0) = CHSV(hue, 255, (255 - x) & 0xFF);
            pixel(actual, 0) = CHSV(hue, 255, 255);
            pixel(actual + 1, 0) = CHSV(hue, 255, x & 0xFF);
            LEDS.show();
            pixel(actual - 1, 0) = CHSV(hue, 0, 0);
            pixel(actual, 0) =  CHSV(hue, 0, 0);
            pixel(actual + 1, 0) =  CHSV(hue, 0, 0);
        }
    }

};


