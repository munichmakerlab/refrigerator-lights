//
//  Plasma.cpp
//  GauntletII
//
//  Created by Robert Atkins on 26/09/13.
//  Copyright (c) 2013 Robert Atkins. All rights reserved.
//

#include "Effect.h"

class Plasma : public Effect {
    
public:
    
    Plasma(CRGB *leds, int width, int height) : Effect(leds, width, height) {
    }
    
    void start() {
        for (uint16_t time = 0, cycles = 0; cycles < 2048; time += 128, cycles++) {
            calcFrame(time);
        }
    }
    //sin(distance(x, y, (128 * sin(-t) + 128), (128 * cos(-t) + 128)) / 40.74)s
    // v = sin(10 * (x * sin(time / 2) + y * cos(time / 3)) + time)
    
    void calcFrame(int time) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int16_t v = 0;
                uint16_t wibble = sin8(time);
                v += sin16(x * wibble * 8 + time);
                v += cos16(y * (128 - wibble) * 4 + time);
                v += sin16(y * x * cos8(-time) / 2);
                
                pixel(x, y) = CHSV((v >> 8) + 127, 255, 255);
            }
        }
        LEDS.show();
    }
    
    uint8_t sin8(uint16_t x) {
        return (sin16(x) >> 8) + 128;
    }
    
    uint8_t cos8(uint16_t x) {
        return (cos16(x) >> 8) + 128;
    }

};


