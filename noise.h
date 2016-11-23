/*
 * refrigerator-lights: https://github.com/munichmakerlab/refrigerator-lights
 * Copyright (C) 2016 Juergen Skrotzky alias Jorgen (JorgenVikingGod@gmail.com)
 *
 * This sketch lights up a 15x10 WS2812b RGB LEDs.
 * The lights are fully controllable over WiFi by MQTT.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef noise_h
#define noise_h

#include "my_mqtt.h"

void resetBack();

void plasma() {
  if (abs(millis() - LastLoop) >= LoopDelayMS) {
    LastLoop = millis();
    resetBack();
    // Fill background with dim plasma
    #define PLASMA_X_FACTOR  24
    #define PLASMA_Y_FACTOR  24
    for (int16_t x=0; x<MATRIX_WIDTH; x++) {        
      for (int16_t y=0; y<MATRIX_HEIGHT; y++) {
        processMQTTLoop();
        if (mqttNewMessage)
          return;
        int16_t r = sin16(PlasmaTime) / 256;
        int16_t h = sin16(x * r * PLASMA_X_FACTOR + PlasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + PlasmaTime) + sin16(y * x * (cos16(-PlasmaTime) / 256) / 2);
        ledsBack(x,y) = CHSV((uint8_t)((h / 256) + 128), 255, 255);
      }
    }
    uint16_t OldPlasmaTime = PlasmaTime;
    PlasmaTime += PlasmaShift;
    if (OldPlasmaTime > PlasmaTime)
      PlasmaShift = (random8(0, 5) * 32) + 64;
  }   
}

#define MAX_DIMENSION ((MATRIX_WIDTH > MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)

// The 16 bit version of our coordinates
static uint16_t noisex;
static uint16_t noisey;
static uint16_t noisez;

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
uint32_t noisespeedx = 1;
uint32_t noisespeedy = 1;
uint32_t noisespeedz = 1;

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise will be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t noisescale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

uint8_t colorLoop = 0;

CRGBPalette16 blackAndWhiteStripedPalette;

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( blackAndWhiteStripedPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  blackAndWhiteStripedPalette[0] = CRGB::White;
  blackAndWhiteStripedPalette[4] = CRGB::White;
  blackAndWhiteStripedPalette[8] = CRGB::White;
  blackAndWhiteStripedPalette[12] = CRGB::White;
}

CRGBPalette16 blackAndGreenStripedPalette;
void SetupBlackAndGreenStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( blackAndGreenStripedPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  blackAndGreenStripedPalette[0] = CRGB::Green;
  blackAndGreenStripedPalette[4] = CRGB::Green;
  blackAndGreenStripedPalette[8] = CRGB::Green;
  blackAndGreenStripedPalette[12] = CRGB::Green;
}

CRGBPalette16 blackAndBlueStripedPalette;

// This function sets up a palette of black and blue stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndBlueStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( blackAndBlueStripedPalette, 16, CRGB::Black);

  for(uint8_t i = 0; i < 6; i++) {
    blackAndBlueStripedPalette[i] = CRGB::Blue;
  }
}

boolean initialized = false;

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {

  if(!initialized) {
    initialized = true;
    // Initialize our coordinates to some random values
    noisex = random16();
    noisey = random16();
    noisez = random16();
  }

  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  uint16_t lowestNoise = noisespeedx < noisespeedy ? noisespeedx : noisespeedy;
  lowestNoise = lowestNoise < noisespeedz ? lowestNoise : noisespeedz;
  if( lowestNoise < 8) {
    dataSmoothing = 200 - (lowestNoise * 4);
  }

  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = noisescale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      yield();
      int joffset = noisescale * j;
      
      processMQTTLoop();
      if (mqttNewMessage)
        return;
                  
      uint8_t data = inoise8(noisex + ioffset, noisey + joffset, noisez);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}

void mapNoiseToLEDsUsingPalette(CRGBPalette16 palette, uint8_t hueReduce = 0)
{
  static uint8_t ihue=0;

  for(int i = 0; i < MATRIX_WIDTH; i++) {
    for(int j = 0; j < MATRIX_HEIGHT; j++) {
      yield();
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.
      processMQTTLoop();
      if (mqttNewMessage)
        return;
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }
      //bri = map(bri, 0,255, 0,64);

      if(hueReduce > 0) {
        if(index < hueReduce) index = 0;
        else index -= hueReduce;
      }

      CRGB color = ColorFromPalette( palette, index, bri);
      ledsBack(i, j) = color;
    }
  }

  ihue+=1;
}

uint16_t drawNoise(CRGBPalette16 palette,uint8_t hueReduce = 0) {
  // generate noise data
  fillnoise8();

  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette(palette, hueReduce);

  return 10;
}

uint16_t rainbowNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(RainbowColors_p);
}

uint16_t rainbowStripeNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 20;
  colorLoop = 0;
  return drawNoise(RainbowStripeColors_p);
}

uint16_t partyNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(PartyColors_p);
}

uint16_t forestNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 120;
  colorLoop = 0;
  return drawNoise(ForestColors_p);
}

uint16_t cloudNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(CloudColors_p);
}

uint16_t fireNoise() {
  noisespeedx = 4; // 24;
  noisespeedy = 0;
  noisespeedz = 4;
  noisescale = 50;
  colorLoop = 0;
  return drawNoise(HeatColors_p, 60);
}

uint16_t lavaNoise() {
  noisespeedx = 8;
  noisespeedy = 0;
  noisespeedz = 4;
  noisescale = 50;
  colorLoop = 0;
  return drawNoise(LavaColors_p);
}

uint16_t oceanNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 90;
  colorLoop = 0;
  return drawNoise(OceanColors_p);
}

uint16_t blackAndWhiteNoise() {
  SetupBlackAndWhiteStripedPalette();
  noisespeedx = std::max(1,second()/4);
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(blackAndWhiteStripedPalette);
}

uint16_t blackAndGreenNoise() {
  SetupBlackAndGreenStripedPalette();
  noisespeedx = -std::max(4,second()/3);
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(blackAndGreenStripedPalette);
}

uint16_t blackAndBlueNoise() {
  SetupBlackAndBlueStripedPalette();
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(blackAndBlueStripedPalette);
}

#endif // noise_h
