//
//  Bouncy.cpp
//  GauntletII
//
//  Created by Robert Atkins on 26/09/13.
//  Copyright (c) 2013 Robert Atkins. All rights reserved.
//

#include "Effect.h"

class Bouncy : public Effect {
    
private:
    
public:
    
    Bouncy(CRGB *leds, int width, int height): Effect(leds, width, height) {
    }
        
    void start() {
      for (int y = 0; y < height; y++) {
        pixel(0, y) = CRGB::White;
        LEDS.show();
        delay(sin16(y * 1024) / 1024);
        pixel(0, y) = CRGB::Black;
        LEDS.show();
      } 
    }
};


