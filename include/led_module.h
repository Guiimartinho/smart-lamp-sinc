#ifndef LED_MODULE_H
#define LED_MODULE_H

#include <FastLED.h>
#include "config.h"

enum lampStates
{
  OFF,
  SOLID,
  STRIPES,
  GRADIENT,
  MOVING_GRADIENT,
  BREATHE,
  RAINBOW
};

enum colorTypes
{
  RGB_COLOR,
  HSV_COLOR
};

//#define LED_SPIRAL
#define LED_STRIP
// Definições para a faixa de LEDs
#ifdef LED_STRIP
const int NUM_LEDS = 48;
const int NUM_STRIPS = 8;
const int LEDS_PER_STRIP = 6;
const int GRADIENT_LENGTH = LEDS_PER_STRIP;
const int STRIPE_WIDTH = LEDS_PER_STRIP;
#endif

#ifdef LED_SPIRAL
const int NUM_LEDS = 60;
const int GRADIENT_LENGTH = NUM_LEDS;
const int STRIPE_WIDTH = 10;
#endif


// Outras configurações
#define RAINBOW_MAX_VALUE 1280

extern lampStates lampState;
extern bool stateChange;
extern int color1[3];
extern int color2[3];
extern colorTypes colorType;


extern CRGB leds[NUM_LEDS];
extern CRGB temp_leds[LEDS_PER_STRIP];
extern CRGB gradient[GRADIENT_LENGTH];

extern int gradientStartPos;
extern int gradientDelta;


// Delay Settings for Patterns
extern unsigned long prevRainbowTime;
extern unsigned long rainbowInterval;
extern unsigned int rainbowCycleCount;
extern unsigned long prevWaveTime;
extern unsigned long waveInterval;
extern unsigned long prevBreatheTime;
extern unsigned long breatheInterval;
extern unsigned long breatheHoldInterval;

void setup_leds();
void loop_leds();

void stripes(int width);
void applyToStrips();
void moving_gradient();
bool breathe(bool firstRun);
void rainbow();
void startup_flash();
void blinkRedLEDs(int blinkCount, int blinkInterval);

#endif
