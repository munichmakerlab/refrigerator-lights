/*
 * Effect implementation for the GauntletII project.
 */

#include "Effect.h"

class Snake : public Effect {
private:
    
    static const byte SNAKE_LENGTH = 16;
    
    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };
    
    struct Pixel {
        uint8_t x;
        uint8_t y;
    };
    
    CRGB colours[SNAKE_LENGTH];
    uint8_t initialHue;
    
    Pixel pixels[SNAKE_LENGTH];
    
    Direction direction;
    
    void newDirection() {
        switch (direction) {
            case UP:
            case DOWN:
                direction = random(0, 2) == 1 ? RIGHT : LEFT;
                break;
                
            case LEFT:
            case RIGHT:
                direction = random(0, 2) == 1 ? DOWN : UP;
                
            default:
                break;
        }
    }
    
    void shuffleDown() {
        for (byte i = SNAKE_LENGTH - 1; i > 0; i--) {
            pixels[i] = pixels[i - 1];
        }
    }
    
public:
    Snake(CRGB *leds, int width, int height) : Effect(leds, width, height), initialHue(0) {
        direction = UP;
        for (int i = 0; i < SNAKE_LENGTH; i++) {
            pixels[i].x = 0;
            pixels[i].y = 0;
        }
    }
    
    void start() {
        clearLeds();
        for (int frameNo = 0; frameNo < 1000; frameNo++) {
            shuffleDown();
            if (random(10) > 6) {
                newDirection();
            }
            switch (direction) {
                case UP:
                    pixels[0].y = (pixels[0].y + 1) % height;
                    break;
                case LEFT:
                    pixels[0].x = (pixels[0].x + 1) % width;
                    break;
                case DOWN:
                    pixels[0].y = pixels[0].y == 0 ? height - 1 : pixels[0].y - 1;
                    break;
                case RIGHT:
                    pixels[0].x = pixels[0].x == 0 ? width - 1 : pixels[0].x - 1;
                    break;
            }
            fill_rainbow(colours, SNAKE_LENGTH, initialHue++);
            for (byte i = 0; i < SNAKE_LENGTH; i++) {
                pixel(pixels[i].x, pixels[i].y) = colours[i] %= (255 - i * (255 / SNAKE_LENGTH));
            }
            LEDS.show();
            delay(30);
            clearLeds();
        }
    }
};

