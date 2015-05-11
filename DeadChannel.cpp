/*
 * Effect implementation for the GauntletII project.
 */

#include "Effect.h"

#define DELAY 32

class DeadChannel : public Effect {
    
private:
    
public:
    DeadChannel(CRGB *leds, int width, int height) : Effect(leds, width, height) {
    }
    
    void start() {
        for (int i = 0; i < 5; i++) {
            playStatic();
            blank();
        }
    }
    
    void playStatic() {
        uint8_t staticLength = random8(24);
        for (int staticFrame = 0; staticFrame < staticLength; staticFrame++) {
            for (int i = 0; i < width * height; i++) {
                leds[i] = CHSV(0, 0, random());
            }
            LEDS.show();
        }
    }
    
    void blank() {
        clearLeds();
        LEDS.show();
        delay(32 * random(16));
    }
};

