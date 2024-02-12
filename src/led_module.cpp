#include "led_module.h"

unsigned long prevRainbowTime = 0;
unsigned long rainbowInterval = 0;
unsigned int rainbowCycleCount = 0;
unsigned long prevWaveTime = 0;
unsigned long waveInterval = 0;
unsigned long prevBreatheTime = 0;
unsigned long breatheInterval = 0;
unsigned long breatheHoldInterval = 0;

lampStates lampState = OFF; // Exemplo inicial
bool stateChange = false;
int color1[3] = {0, 0, 0};
int color2[3] = {0, 0, 0};
colorTypes colorType = RGB_COLOR;

CRGB leds[NUM_LEDS];
CRGB temp_leds[LEDS_PER_STRIP];
CRGB gradient[GRADIENT_LENGTH];

int gradientStartPos = 0;
int gradientDelta = 1;

void setup_leds()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  // Inicialização adicional, se necessária
  lampState = OFF;
  FastLED.clear();
  FastLED.show();

  startup_flash();
}

void loop_leds()
{
  switch (lampState)
  {

  case OFF:
  {
    if (stateChange)
    {
      FastLED.clear();
      FastLED.show();
      Serial.println("LED to off");
    }
    break;
  }

  case SOLID:
  {
    if (stateChange)
    {
      if (colorType == RGB_COLOR)
        fill_solid(leds, NUM_LEDS, CRGB(color1[0], color1[1], color1[2]));

      else if (colorType == HSV_COLOR)
        fill_solid(leds, NUM_LEDS, CHSV(color1[0], color1[1], color1[2]));

      FastLED.show();
      Serial.println("LED to solid");
    }
    break;
  }

  case STRIPES:
  {
    if (stateChange)
    {
      stripes(STRIPE_WIDTH);
      Serial.println("LED to stripes");
    }
    break;
  }

  case GRADIENT:
  {
    if (stateChange)
    {
      if (colorType == RGB_COLOR)
      {
#ifdef LED_STRIP
        fill_gradient_RGB(temp_leds, 0, CRGB(color1[0], color1[1], color1[2]), LEDS_PER_STRIP - 1, CRGB(color2[0], color2[1], color2[2]));
#endif
#ifdef LED_SPIRAL
        fill_gradient_RGB(leds, 0, CRGB(color1[0], color1[1], color1[2]), NUM_LEDS - 1, CRGB(color2[0], color2[1], color2[2]));
#endif
      }

      else if (colorType == HSV_COLOR)
      {
#ifdef LED_STRIP
        fill_gradient(temp_leds, LEDS_PER_STRIP, CHSV(color1[0], color1[1], color1[2]), CHSV(color2[0], color2[1], color2[2]), SHORTEST_HUES);
#endif
#ifdef LED_SPIRAL
        fill_gradient(leds, NUM_LEDS, CHSV(color1[0], color1[1], color1[2]), CHSV(color2[0], color2[1], color2[2]), SHORTEST_HUES);
#endif
      }
      applyToStrips();
      FastLED.show();
      Serial.println("LED to gradient");
    }
    break;
  }

  case MOVING_GRADIENT:
  {
    unsigned long currentMillis = millis();
    if (currentMillis - prevWaveTime > waveInterval)
    {
      prevWaveTime = currentMillis;
      moving_gradient();
    }
    break;
  }

  case BREATHE:
  {
    static bool holdColor = false;
    unsigned long currentMillis = millis();

    if (!holdColor)
    {
      unsigned long interval = (color1[2] > 0) ? (breatheInterval / color1[2]) : 10;
      if (currentMillis - prevBreatheTime > interval)
      {
        prevBreatheTime = currentMillis;
        holdColor = breathe(stateChange);
      }
    }
    else
    {
      if (currentMillis - prevBreatheTime > breatheHoldInterval)
      {
        prevBreatheTime = currentMillis;
        holdColor = breathe(stateChange);
      }
    }

    break;
  }

  case RAINBOW:
  {
    if (stateChange || (rainbowCycleCount >= RAINBOW_MAX_VALUE))
    {
      rainbowCycleCount = 0;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - prevRainbowTime > rainbowInterval)
    {
      prevRainbowTime = currentMillis;
      rainbow();
      rainbowCycleCount++;
    }
    break;
  }

  default:
  {
    break;
  }
  }
  stateChange = false;
}

// Display alternating stripes
void stripes(int width)
{
  if (colorType == RGB_COLOR)
  {
    CRGB c1 = CRGB(color1[0], color1[1], color1[2]);
    CRGB c2 = CRGB(color2[0], color2[1], color2[2]);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (i % (width * 2) < width)
      {
        leds[i] = c1;
      }
      else
      {
        leds[i] = c2;
      }
    }
  }

  else if (colorType == HSV_COLOR)
  {
    CHSV c1 = CHSV(color1[0], color1[1], color1[2]);
    CHSV c2 = CHSV(color2[0], color2[1], color2[2]);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (i % (width * 2) < width)
      {
        leds[i] = c1;
      }
      else
      {
        leds[i] = c2;
      }
    }
  }
  FastLED.show();
}

// Corrects the LED patterns for the strip configuration
void applyToStrips()
{
  int led_index = 0;
  for (int j = 0; j < NUM_STRIPS; j++)
  {
    if (j % 2 == 0)
    {
      for (int i = 0; i < LEDS_PER_STRIP; i++)
      {
        leds[led_index] = temp_leds[i];
        led_index++;
      }
    }
    else
    {
      for (int i = LEDS_PER_STRIP - 1; i >= 0; i--)
      {
        leds[led_index] = temp_leds[i];
        led_index++;
      }
    }
  }
}

// Moving Gradient
void moving_gradient()
{
#ifdef LED_SPIRAL
  uint8_t count = 0;
  for (uint8_t i = gradientStartPos; i < gradientStartPos + GRADIENT_LENGTH; i++)
  {
    leds[i % NUM_LEDS] = gradient[count];
    count++;
  }

  FastLED.show();
  FastLED.clear();

  gradientStartPos = gradientStartPos + gradientDelta; // Update start position.
  if ((gradientStartPos > NUM_LEDS - 1) || (gradientStartPos < 0))
  {                                                 // Check if outside NUM_LEDS range
    gradientStartPos = gradientStartPos % NUM_LEDS; // Loop around as needed.
  }
#endif

#ifdef LED_STRIP
  uint8_t count = 0;
  for (uint8_t i = gradientStartPos; i < gradientStartPos + GRADIENT_LENGTH; i++)
  {
    temp_leds[i % LEDS_PER_STRIP] = gradient[count];
    count++;
  }
  applyToStrips();
  FastLED.show();
  FastLED.clear();

  gradientStartPos = gradientStartPos + gradientDelta; // Update start position.
  if ((gradientStartPos > LEDS_PER_STRIP - 1) || (gradientStartPos < 0))
  {                                                       // Check if outside NUM_LEDS range
    gradientStartPos = gradientStartPos % LEDS_PER_STRIP; // Loop around as needed.
  }
#endif
}
// Breathing
bool breathe(bool firstRun)
{
  static int value;
  static int fade;
  static long prevMillis;
  bool holdColor;

  if (firstRun)
  {
    fade = (color1[2] > 100) ? 5 : 1;
    value = fade;
  }

  if ((value <= color1[2]) && (value >= 0))
  {
    holdColor = false;
    fill_solid(leds, NUM_LEDS, CHSV(color1[0], color1[1], value));
  }
  else if (value > color1[2])
  {
    holdColor = true;
    fade = -fade;
  }
  else if (value < 0)
  {
    holdColor = true;
    fade = -fade;
  }

  FastLED.show();
  value += fade;
  return holdColor;
}

CRGB Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor()
{
  return Wheel(random(256));
}
// Rainbow colors that slowly cycle across LEDs
void rainbow()
{
#ifdef LED_SPIRAL
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = Wheel(((i * 256 / NUM_LEDS) + rainbowCycleCount) & 255);
  }
#endif
#ifdef LED_STRIP
  for (int i = 0; i < LEDS_PER_STRIP; i++)
  {
    temp_leds[i] = Wheel(((i * 256 / LEDS_PER_STRIP) + rainbowCycleCount) & 255);
  }
  applyToStrips();
#endif
  FastLED.show();
}

// Startup Flash
void startup_flash()
{
  for (int i = 0; i < 3; i++)
  {
    fill_solid(leds, NUM_LEDS, CHSV(0, 0, 10));
    FastLED.show();
    delay(200);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(200);
  }
}

// Função para piscar LEDs em vermelho
void blinkRedLEDs(int blinkCount, int blinkInterval) {
    for (int i = 0; i < blinkCount; i++) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        delay(blinkInterval / 2);
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(blinkInterval / 2);
    }
}