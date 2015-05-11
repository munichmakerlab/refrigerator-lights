//
//  TestPattern.cpp
//  GauntletII
//
//  Created by Robert Atkins on 26/09/13.
//  Copyright (c) 2013 Robert Atkins. All rights reserved.
//

#include "Effect.h"

class TestPattern : public Effect {
    
public:
    
    TestPattern(CRGB *leds, int width, int height): Effect(leds, width, height) {
    }
    
    void start() {
//        Serial.print("Hello, white!");
//        chasePixels(CRGB::White);

        Serial.print("Hello, red!");
        chasePixels(CRGB::Red);
//        
        Serial.print("Hello, green!");
        chasePixels(CRGB::Green);
//        
        Serial.print("Hello, blue!");        
        chasePixels(CRGB::Blue);
        
//        blockPixels(CRGB::Red);
//        blockPixels(CRGB::Green);
//        blockPixels(CRGB::Blue);
//        
//        for (int i = 0; i < 256; i += 16) {
//            blockPixels(CRGB::White, i);
//        }
//        blockPixels(CRGB::White);
    }
    
    void chasePixels(CRGB colour) {
        for (int pixel = 0; pixel < width * height; pixel++) {
            leds[pixel] = colour;
            LEDS.show();
            leds[pixel] = CRGB::Black;
        }
    }
    
    void blockPixels(CRGB color) {
        LEDS.showColor(color);
        delay(2000);
    }
    
    void blockPixels(CRGB color, byte intensity) {
        LEDS.showColor(color, intensity);
        delay(2000);
    }
    
};


