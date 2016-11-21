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

#ifndef tetris_h
#define tetris_h

#include "config.h"

#define TARGET_FRAME_TIME    25  // Desired update rate, though if too many leds it will just run as fast as it can!
#define INITIAL_DROP_FRAMES  20  // Start of game block drop delay in frames

// Joystick pins used with pullup so active when grounded
#define ROTATE_PIN  3
#define LEFT_PIN    4
#define RIGHT_PIN   5
#define DOWN_PIN    6

const uint8_t TetrisIData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(11110000),
  // Frame 2
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(11110000),
  // Frame 4
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000),
  B8_3BIT(10000000)
};
const uint8_t TetrisIMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11110000),
  // Frame 2
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11110000),
  // Frame 4
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisJData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(22200000),
  B8_3BIT(00200000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(02000000),
  B8_3BIT(02000000),
  B8_3BIT(22000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(20000000),
  B8_3BIT(22200000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(22000000),
  B8_3BIT(20000000),
  B8_3BIT(20000000)
};
const uint8_t TetrisJMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisLData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(33300000),
  B8_3BIT(30000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(33000000),
  B8_3BIT(03000000),
  B8_3BIT(03000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(00300000),
  B8_3BIT(33300000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(30000000),
  B8_3BIT(30000000),
  B8_3BIT(33000000)
};
const uint8_t TetrisLMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(10000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000)
};
const uint8_t TetrisOData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(44000000),
  B8_3BIT(44000000)
};
const uint8_t TetrisOMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(11000000)
};
const uint8_t TetrisSData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(05500000),
  B8_3BIT(55000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(50000000),
  B8_3BIT(55000000),
  B8_3BIT(05000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(05500000),
  B8_3BIT(55000000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(50000000),
  B8_3BIT(55000000),
  B8_3BIT(05000000)
};
const uint8_t TetrisSMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01100000),
  B8_1BIT(11000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01100000),
  B8_1BIT(11000000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000)
};
const uint8_t TetrisTData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(66600000),
  B8_3BIT(06000000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(06000000),
  B8_3BIT(66000000),
  B8_3BIT(06000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(06000000),
  B8_3BIT(66600000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(60000000),
  B8_3BIT(66000000),
  B8_3BIT(60000000)
};
const uint8_t TetrisTMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11100000),
  B8_1BIT(01000000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(01000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(10000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000)
};
const uint8_t TetrisZData[] = 
{
  // Frame 1
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(77000000),
  B8_3BIT(07700000),
  // Frame 2
  B8_3BIT(00000000),
  B8_3BIT(07000000),
  B8_3BIT(77000000),
  B8_3BIT(70000000),
  // Frame 3
  B8_3BIT(00000000),
  B8_3BIT(00000000),
  B8_3BIT(77000000),
  B8_3BIT(07700000),
  // Frame 4
  B8_3BIT(00000000),
  B8_3BIT(07000000),
  B8_3BIT(77000000),
  B8_3BIT(70000000)
};
const uint8_t TetrisZMask[] = 
{
  // Frame 1
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01100000),
  // Frame 2
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000),
  // Frame 3
  B8_1BIT(00000000),
  B8_1BIT(00000000),
  B8_1BIT(11000000),
  B8_1BIT(01100000),
  // Frame 4
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(11000000),
  B8_1BIT(10000000)
};

#define TETRIS_SPR_WIDTH  4
#define TETRIS_SPR_HEIGHT 4
const uint8_t *TetrisSprData[] = { TetrisIData, TetrisJData, TetrisLData, TetrisOData, TetrisSData, TetrisTData, TetrisZData };
const uint8_t *TetrisSprMask[] = { TetrisIMask, TetrisJMask, TetrisLMask, TetrisOMask, TetrisSMask, TetrisTMask, TetrisZMask};
const struct CRGB TetrisColours[] = { CRGB(0, 255, 255), CRGB(0, 0, 255), CRGB(255, 165, 0), CRGB(255, 255, 0), CRGB(50, 205, 50), CRGB(255, 0, 255), CRGB(255, 0, 0) };

uint8_t PlayfieldData[MATRIX_HEIGHT * ((MATRIX_WIDTH + 7) / 8) * _3BIT];
uint8_t PlayfieldMask[MATRIX_HEIGHT * ((MATRIX_WIDTH + 7) / 8) * _1BIT];
uint8_t CompletedLinesData[TETRIS_SPR_HEIGHT * ((MATRIX_WIDTH + 7) / 8) * _1BIT];
const struct CRGB CompletedLinesColour[] = { CRGB(255, 255, 255) };
cSprite Playfield, CompletedLines, CurrentBlock;
cLEDSprites Sprites(&tetrisLeds);

unsigned char AttractMsg[144], GameOverMsg[88];
char BlankMsg[32];
cLEDText TetrisMsg;

uint8_t DropDelay;
boolean AttractMode, NextBlock;
int16_t TotalLines;
unsigned int HighScore = 0, LastScore;

// Joystick class to handle input debounce along with variable delays and repeat option
class cJoyStick
{
public:
  cJoyStick(uint8_t pin, uint16_t Debouncems, uint16_t Delayms, boolean Repeat)
  {
    m_pin = pin;
    m_DebounceMS = Debouncems;
    m_DelayMS = Delayms;
    m_Repeat = Repeat;
    m_LastMS = millis();
    m_state = digitalRead(m_pin);
  }
  boolean Read()
  {
    /*
    uint8_t state = digitalRead(m_pin);
    uint32_t ms = millis();
    if ((state != m_state) && ((ms - m_LastMS) >= m_DebounceMS))
    {
      m_state = state;
      m_LastMS = ms;
      return(true);
    }
    if ( (m_Repeat) && ((ms - m_LastMS) >= m_DelayMS) )
    {
      m_LastMS = ms;
      return(true);
    }
    return(false);
    */
    return(true);
  }
  void SetState(uint8_t state) 
  {
    m_state = state;
  }
  uint8_t GetState()
  {
    return(m_state);
  }
  void SetRepeat(boolean Repeat)
  {
    m_Repeat = Repeat;
  }
protected:
  uint8_t m_pin, m_state;
  uint16_t m_DebounceMS, m_DelayMS;
  uint32_t m_LastMS;
  boolean m_Repeat;
};

cJoyStick JSRotate(ROTATE_PIN, 10, 250, false);
cJoyStick JSLeft(LEFT_PIN, 10, 250, false);
cJoyStick JSRight(RIGHT_PIN, 10, 250, false);
cJoyStick JSDown(DOWN_PIN, 10, 50, false);

void tetrisSetup() {
  tetrisLeds.SetLEDArray(leds[0]);
  memset(PlayfieldData, 0, sizeof(PlayfieldData));
  memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
  Playfield.Setup(tetrisLeds.Width(), tetrisLeds.Height(), PlayfieldData, 1, _3BIT, TetrisColours, PlayfieldMask);
  Playfield.SetPositionFrameMotionOptions(0, 0, 0, 0, 0, 0, 0, 0, 0);
  Sprites.AddSprite(&Playfield);

  memset(CompletedLinesData, 0, sizeof(CompletedLinesData));
  CompletedLines.Setup(tetrisLeds.Width(), TETRIS_SPR_HEIGHT, CompletedLinesData, 1, _1BIT, CompletedLinesColour, CompletedLinesData);
  CompletedLines.SetPositionFrameMotionOptions(0, 0, 0, 0, 0, 0, 0, 0, 0);

  TetrisMsg.SetFont(MatriseFontData);
  sprintf((char *)BlankMsg, "%.*s", std::min(((tetrisLeds.Width() + TetrisMsg.FontWidth()) / (TetrisMsg.FontWidth() + 1)), (int)sizeof(BlankMsg) - 1), "                              ");
  sprintf((char *)AttractMsg, "%sTETRIS%sSCORE %u%sHIGH %u%sANY BUTTON TO START%s", BlankMsg, BlankMsg, LastScore, BlankMsg, (int)HighScore, BlankMsg, BlankMsg);
  TetrisMsg.Init(&tetrisLeds, tetrisLeds.Width(), tetrisLeds.Height(), 0, (-tetrisLeds.Height() + TetrisMsg.FontHeight()) / 2);
  TetrisMsg.SetBackgroundMode(BACKGND_LEAVE);
  TetrisMsg.SetFrameRate(2);
  TetrisMsg.SetOptionsChangeMode(INSTANT_OPTIONS_MODE);
  TetrisMsg.SetText(AttractMsg, strlen((const char *)AttractMsg));
  AttractMode = true;
  LoopDelayMS = TARGET_FRAME_TIME;
  LastLoop = millis() - LoopDelayMS;
  PlasmaShift = (random8(0, 5) * 32) + 64;
  PlasmaTime = 0;
}

void tetrisLoop() {
  if (abs(millis() - LastLoop) >= LoopDelayMS)
  {
    LastLoop = millis();
    FastLED.clear();

    // Fill background with dim plasma
    #define PLASMA_X_FACTOR  24
    #define PLASMA_Y_FACTOR  24
    for (int16_t x=0; x<MATRIX_WIDTH; x++) {
      for (int16_t y=0; y<MATRIX_HEIGHT; y++) {
        yield();
        int16_t r = sin16(PlasmaTime) / 256;
        int16_t h = sin16(x * r * PLASMA_X_FACTOR + PlasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + PlasmaTime) + sin16(y * x * (cos16(-PlasmaTime) / 256) / 2);
        tetrisLeds(x, y) = CHSV((uint8_t)((h / 256) + 128), 255, 64);
      }
    }
    uint16_t OldPlasmaTime = PlasmaTime;
    PlasmaTime += PlasmaShift;
    if (OldPlasmaTime > PlasmaTime)
      PlasmaShift = (random8(0, 5) * 32) + 64;

    if (AttractMode)
    {
      if ( ((JSRotate.Read()) && (JSRotate.GetState() == LOW)) || ((JSLeft.Read()) && (JSLeft.GetState() == LOW))
          || ((JSRight.Read()) && (JSRight.GetState() == LOW)) || ((JSDown.Read()) && (JSDown.GetState() == LOW)) )
      {
        JSRotate.SetState(HIGH);
        JSLeft.SetState(HIGH);
        JSRight.SetState(HIGH);
        JSDown.SetState(HIGH);
        
        JSRotate.SetRepeat(true);
        JSLeft.SetRepeat(true);
        JSRight.SetRepeat(true);
        JSDown.SetRepeat(true);
        AttractMode = false;
        memset(PlayfieldData, 0, sizeof(PlayfieldData));
        memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
        Sprites.RemoveSprite(&CurrentBlock);
        LastScore = 0;
        TotalLines = 0;
        DropDelay = INITIAL_DROP_FRAMES;
        CurrentBlock.SetXChange(-1);
        NextBlock = true;
      }
    }
    else
    {
      if (Sprites.IsSprite(&CompletedLines))  // We have highlighted complete lines, delay for visual effect
      {
        if (CompletedLines.GetXCounter() > 0)
          CompletedLines.SetXCounter(CompletedLines.GetXCounter() - 1);
        else
        {
          Sprites.RemoveSprite(&CompletedLines);
          // Remove completed lines from playfield sprite
          uint8_t *Data = PlayfieldData;
          uint8_t *Mask = PlayfieldMask;
          uint16_t Mbpl = (MATRIX_WIDTH + 7) / 8;
          uint16_t Dbpl = Mbpl * _3BIT;
          int16_t k;
          // strip out bottom line of all sprites
          for (int16_t i=(MATRIX_HEIGHT-1)*Dbpl,j=(MATRIX_HEIGHT-1)*Mbpl; i>=0; i-=Dbpl,j-=Mbpl) {
            for (k=0; k<MATRIX_WIDTH; k+=8) {
              yield();
              if ((uint8_t)(0xff00 >> std::min(MATRIX_WIDTH - k, 8)) != Mask[j + (k / 8)])
                break;
            }
            if (k >= MATRIX_WIDTH) {
              memmove(&Data[Dbpl], &Data[0], i);
              memset(&Data[0], 0, Dbpl);
              memmove(&Mask[Mbpl], &Mask[0], j);
              memset(&Mask[0], 0, Mbpl);
              i+=Dbpl;
              j+=Mbpl;
            }
          }
        }
      } else {
        // We have a current block 
        if (CurrentBlock.GetXChange() >= 0) {
          // Check for user input
          if ( (JSRotate.Read()) && (JSRotate.GetState() == LOW) ) {
            JSRotate.SetState(HIGH);
            
            if ((CurrentBlock.GetCurrentFrame() % 2) == 1) {
              if (CurrentBlock.GetXChange() == 0)
                CurrentBlock.m_X = std::min((int)(CurrentBlock.m_X), (MATRIX_WIDTH - TETRIS_SPR_WIDTH));
              else if ((CurrentBlock.GetXChange() != 3) && (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MAX))
                --CurrentBlock.m_X;
            }
            CurrentBlock.IncreaseFrame();
            Sprites.DetectCollisions(&CurrentBlock);
            if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
              CurrentBlock.DecreaseFrame();
          }
          if ( (JSLeft.Read()) && (JSLeft.GetState() == LOW) && (! (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MIN)) ) {
            JSLeft.SetState(HIGH);
            CurrentBlock.m_X--;
            Sprites.DetectCollisions(&CurrentBlock);
            if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
              CurrentBlock.m_X++;
          } else if ( (JSRight.Read()) && (JSRight.GetState() == LOW) && (! (CurrentBlock.GetFlags() & SPRITE_EDGE_X_MAX)) ) {
            JSRight.SetState(HIGH);
            CurrentBlock.m_X++;
            Sprites.DetectCollisions(&CurrentBlock);
            if (CurrentBlock.GetFlags() & SPRITE_COLLISION)
              CurrentBlock.m_X--;
          }
          if ( (JSDown.Read()) && (JSDown.GetState() == LOW) ) {;
            JSDown.SetState(HIGH); 
            CurrentBlock.SetYCounter(1);
          }
          // Do block checks for bottom or collision
          if (CurrentBlock.GetYCounter() <= 1) {
            if (CurrentBlock.GetFlags() & SPRITE_EDGE_Y_MIN)
              NextBlock = true;
            else {
              --CurrentBlock.m_Y;
              Sprites.DetectCollisions(&CurrentBlock);
              ++CurrentBlock.m_Y;
              if (CurrentBlock.GetFlags() & SPRITE_COLLISION) {
                // Block has collided check for game over
                int16_t MaxY = MATRIX_HEIGHT - 2;
                if ((CurrentBlock.GetCurrentFrame() % 2) == 1) {
                  if (CurrentBlock.GetXChange() == 0)
                    MaxY -= 2;
                  else if (CurrentBlock.GetXChange() != 3)
                    MaxY -= 1;
                } else if (CurrentBlock.GetXChange() == 0)
                    ++MaxY;
                if (CurrentBlock.m_Y < MaxY)
                  NextBlock = true;
                else {
                  // Game over
                  CurrentBlock.SetYCounter(2);  // Stop last block moving down!
                  AttractMode = true;
                  JSRotate.SetRepeat(false);
                  JSLeft.SetRepeat(false);
                  JSRight.SetRepeat(false);
                  JSDown.SetRepeat(false);
                  if (LastScore > HighScore) {
                    HighScore = LastScore;
                    sprintf((char *)GameOverMsg, "%sGAME OVER%sNEW HIGH SCORE %u%s", BlankMsg, BlankMsg, LastScore, BlankMsg);
                  } else
                    sprintf((char *)GameOverMsg, "%sGAME OVER%sSCORE %u%s", BlankMsg, BlankMsg, LastScore, BlankMsg);
                  sprintf((char *)AttractMsg, "%sTETRIS%sSCORE %u%sHIGH %u%sANY BUTTON TO START%s", BlankMsg, BlankMsg, LastScore, BlankMsg, HighScore, BlankMsg, BlankMsg);
                  
                  TetrisMsg.SetText(GameOverMsg, strlen((char *)GameOverMsg));
                  TetrisMsg.SetBackgroundMode(BACKGND_DIMMING, 0x40);
                  DEBUG_PRINTLN("Game Over");
                  publishMQTTMessage(strTopicPrefix + "game/tetris", String("Game Over - score ") + String(LastScore));
                }
              }
            }
          }
        }
        // Start new block
        if (NextBlock) {
          // We have a current block so add to playfield before creating new block 
          if (CurrentBlock.GetXChange() >= 0) {
            Playfield.Combine(CurrentBlock.m_X, CurrentBlock.m_Y, &CurrentBlock);
            Sprites.RemoveSprite(&CurrentBlock);
            // Make completed lines highlight sprite & score
            memset(CompletedLinesData, 0, sizeof(CompletedLinesData));
            CompletedLines.m_Y = -1;
            uint8_t *Mask = PlayfieldMask;
            uint16_t Mbpl = (MATRIX_WIDTH + 7) / 8;
            int16_t j, numlines = 0;
            for (int16_t i=(MATRIX_HEIGHT-1)*Mbpl, y=0; i>=0; i-=Mbpl,++y) {
              for (j=0; j<MATRIX_WIDTH; j+=8) {
                yield();
                if ((uint8_t)(0xff00 >> std::min(MATRIX_WIDTH - j, 8)) != Mask[i + (j / 8)])
                  break;
              }
              if (j >= MATRIX_WIDTH) {
                if (CompletedLines.m_Y == -1)
                  CompletedLines.m_Y = y;
                memset(&CompletedLinesData[((TETRIS_SPR_HEIGHT - 1) - (y - CompletedLines.m_Y)) * Mbpl], 0xff, Mbpl);
                numlines++;
              }
            }
            if (numlines > 0) {
              CompletedLines.SetXCounter(15);  // Set delay for highlight display to 15 loops
              Sprites.AddSprite(&CompletedLines);
            }
            LastScore += 1;
            if (numlines == 1)
              LastScore += 4;
            else if (numlines == 2)
              LastScore += 12;
            else if (numlines == 3)
              LastScore += 20;
            else if (numlines == 4)
              LastScore += 40;
            TotalLines += numlines;
            DropDelay = std::max(1, INITIAL_DROP_FRAMES - (TotalLines / 5));
          }
          // Start new block
          uint8_t j = random8(sizeof(TetrisSprData) / sizeof(TetrisSprData[0]));
          CurrentBlock.Setup(TETRIS_SPR_WIDTH, TETRIS_SPR_WIDTH, TetrisSprData[j], 4, _3BIT, TetrisColours, TetrisSprMask[j]);
          CurrentBlock.SetPositionFrameMotionOptions((MATRIX_WIDTH/2)-1, MATRIX_HEIGHT, 0, 0, 0, 0, -1, DropDelay, SPRITE_DETECT_COLLISION | SPRITE_DETECT_EDGE);
          CurrentBlock.SetXChange(j);
          Sprites.AddSprite(&CurrentBlock);
          NextBlock = false;
        }
        Sprites.UpdateSprites();
      }
    }
    Sprites.RenderSprites();
    if (AttractMode) {
      if (TetrisMsg.UpdateText() == -1) {
        TetrisMsg.SetText(AttractMsg, strlen((char *)AttractMsg));
        TetrisMsg.SetBackgroundMode(BACKGND_LEAVE);
        Sprites.RemoveSprite(&CurrentBlock);
        memset(PlayfieldData, 0, sizeof(PlayfieldData));
        memset(PlayfieldMask, 0, sizeof(PlayfieldMask));
      }
    }
    // This is used in loop for power managed display of LED's.
    show_at_max_brightness_for_power(); 
  }
}

#endif //tetris_h
