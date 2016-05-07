void init_leds()
{
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowColors_p;
  currentBlending = BLEND;
  leds_icon(1); light_swirl(); leds_icon(0); 
}

void light_swirl()
{
  for (int j = 0; j < 255; j++)
  {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* SPEED */
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000/100);
  }

  for (int j = 0; j < 255; j++)
  {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* SPEED */
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000/100);
  }
  
  for (int j = 0; j < 255; j++)
  {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* SPEED */
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000/100);
  }
  
  for (int t = 0; t < NUM_LEDS; t++)
  {
    leds[t] = CRGB::Black;
  }
  FastLED.show();
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
  uint8_t brightness = 255;
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}




