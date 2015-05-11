//
//  Boxes.cpp
//  GauntletII
//
//  Created by Robert Atkins on 26/09/13.
//  Copyright (c) 2013 Robert Atkins. All rights reserved.
//

#include "Effect.h"

class Boxes : public Effect {
    
private:
    
    struct Box {
        uint16_t height, width;
        uint16_t xPos, yPos;
        uint8_t hue;
        uint8_t speed;
    };
    
    uint16_t hiWidth, hiHeight;

public:
    
    Boxes(CRGB *leds, int width, int height): Effect(leds, width, height),
        hiWidth(256 * width),
        hiHeight(256 * height)
    {
    }
        
    void start() {
        Box box;
//        box.height = random(2, 6);
//        box.width = random(3, 8);
//        box.xPos = -box.width;
//        box.yPos = random(box.height, height);
//        box.speed = random(32, 64);
        
        box.height = 1;
        box.width = 3 * 256;
        box.xPos = 0;
        box.yPos = 5;
        box.speed = 16;
        
        hi(box);
    }
    //
    // x x x x | x x x x | x x x x | x x x x
    
    void hi(Box b) {
        //int y = b.yPos;
        for (uint16_t x = b.xPos; x < hiWidth; x+= b.speed) {
            
            clearLeds();
            
            for (int boxwidth = 0; boxwidth <= b.width; boxwidth++) {
                uint16_t actualPixel = (x + boxwidth) >> 8;
                uint16_t actualBrightness = (x + boxwidth) & 0xFF;
                renderIfVisible(actualPixel, 0, CHSV(b.hue, 255, 255 - actualBrightness));
            }
            
            LEDS.show();
        }
    }

    //                renderIfVisible((x >> 8), y, CHSV(b.hue, 255, 255 - x & 0xFF));
    //                renderIfVisible((x + b.width) >> 8, y, CHSV(b.hue, 255, x & 0xFF));

    void renderIfVisible(uint16_t x, uint16_t y, CHSV chsv) {
        if (inXRange(x) && inYRange(y)) {
            pixel(x, y) += chsv;
        }
    }

};


