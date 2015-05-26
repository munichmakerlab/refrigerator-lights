/*
 * Effect implementation for the GauntletII project.
 */

#include "Effect.h"

#define MAX_TWINKS 25
#define OFFSET 0xB000

typedef struct Twink {
    short x;
    short y;
} Twink;

class Twinkle : public Effect {
    
private:
    short numTwinks;
    bool colour;
    bool cycleSaturation;
    
public:
    Twinkle(CRGB *leds, int width, int height, bool colour, bool cycleSaturation) :
        Effect(leds, width, height),
        numTwinks(0),
        colour(colour),
        cycleSaturation(cycleSaturation) {
    }
    
    void start() {
        for (uint16_t frame = 0x0000, iterations = 0; iterations < 2250; frame += 0x20, iterations++) {
            for (int i = 0; i < width * height; i++) {
                if (leds[i]) {
                    leds[i].fadeToBlackBy(50);
                    if (!leds[i]) {
                        numTwinks--;
                    }
                } else {
                    if (random(56) == 0) {
                        numTwinks++;
                        if (colour) {
                            if (cycleSaturation) {
                                uint8_t saturation = (sin16(frame + OFFSET) >> 8) + 128;
                                leds[i] = CHSV(random(255), saturation, 255);
                            } else {
                                leds[i] = CHSV(random(255), random(128, 255), 255);
                            }
                        } else {
                            leds[i] = CRGB::White;
                        }
                    }
                }
            }
            FastLED.show();
        }
    }
};

