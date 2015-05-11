/*
 * The Effect implementation for the TechnocolourDreamcoat project.
 */

#include "Effect.h"

Effect::Effect(CRGB *leds, int width, int height): leds(leds), width(width), height(height) {}

bool Effect::inXRange(int x) {
    return x >= 0 && x < width;
}

bool Effect::inYRange(int y) {
    return y >= 0 && y < height;
}

struct CRGB& Effect::pixel(int x, int y) {
    if (y & 1) {
        return leds[(y * width) + width - x - 1];
    } else {
        return leds[(y * width) + x];
    }
}

void Effect::clearLeds() {
    memset8(leds, 0, width * height * 3);
}
