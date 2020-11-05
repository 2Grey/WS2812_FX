//------------------------------------- UTILITY FXNS --------------------------------------
//---SET THE COLOR OF A LED

void setPixelRGB(int idx, int red, int green, int blue) {
  strip.setRGB(idx, red, green, blue);
}

void setPixelHSV(int idx, int hue, int saturation, int value) {
  strip.setHSV(idx, hue, saturation, value);
}

void setStripRGB(int red, int green, int blue) {
  for (int i = 0; i < LED_COUNT; i++) {
    setPixelRGB(i, red, green, blue);
  }
}

void setStripHSV(int hue, int saturation, int value) {
  for (int i = 0; i < LED_COUNT; i++) {
    setPixelHSV(i, hue, saturation, value);
  }
}

//---FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {
    return BOTTOM_INDEX;
  }
  if (i == TOP_INDEX && EVENODD == 1) {
    return TOP_INDEX + 1;
  }
  if (i == TOP_INDEX && EVENODD == 0) {
    return TOP_INDEX;
  }
  return LED_COUNT - i;
}

//---FIND INDEX OF ANTIPODAL OPPOSITE LED
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = iN % LED_COUNT;
  }
  return iN;
}

//---FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  return (i < LED_COUNT - 1) ? i + 1 : 0;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  return (i > 0) ? i + 1 : LED_COUNT - 1;
}

#pragma mark - Delay and Break

boolean safeDelayWait(uint32_t delayTime) {
  uint32_t thisTime = millis();
  while (millis() - thisTime <= delayTime) {
    if (breakFlag) {
      breakFlag = false;
      return true;
    }
  }
  return false;
}

boolean safeDelayAndBreak(int delayTime) {
  if (breakFlag) {
    breakFlag = false;
    waitTime = millis();
    return true;
  }

  if (millis() - waitTime >= delayTime) {
    waitTime = millis();
    return true;
  }
  return false;
}

//--- Fill rainbow

void fill_rainbox(LEDdata * targetArray, int numToFill, uint8_t initialHue, int hueStep) {
  byte hue = initialHue;

  for (int i = 0; i < numToFill; i++) {
    targetArray[i] = mHSV(hue, 240, 255);
    hue += hueStep;
  }
}
