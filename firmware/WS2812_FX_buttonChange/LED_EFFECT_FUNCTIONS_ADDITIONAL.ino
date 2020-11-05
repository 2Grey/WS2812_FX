//------------------------LED EFFECT FUNCTIONS------------------------
//----------------------Using the ledsX variable----------------------

#if IS_ADDITIONAL_FX
//-m13-1D CELLULAR AUTOMATA - RULE 30 (RED FOR NOW)
void rule30() {
  copy_led_array();
  
  if (bouncedirection == 0) {
    random_red();
    bouncedirection = 1;
  }
  
  int iCW;
  int iCCW;
  int y = 100;

  for (int i = 0; i < LED_COUNT; i++ ) {
    iCW = adjacent_cw(i);
    iCCW = adjacent_ccw(i);

    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] > y) {
      setPixelRGB(i, 0, 0, 0);
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {
      setPixelRGB(i, 0, 0, 0);
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {
      setPixelRGB(i, 0, 0, 0);
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {
      setPixelRGB(i, 255, 0, 0);
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] > y) {
      setPixelRGB(i, 255, 0, 0);
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {
      setPixelRGB(i, 255, 0, 0);
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {
      setPixelRGB(i, 255, 0, 0);
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {
      setPixelRGB(i, 0, 0, 0);
    }
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

//-m14-RANDOM MARCH CCW
void random_march() {
  copy_led_array();
  int iCCW;
  setPixelHSV(0, random(0, 255), 255, 255);

  for (int idex = 1; idex < LED_COUNT ; idex++ ) {
    iCCW = adjacent_ccw(idex);
    setPixelRGB(idex, ledsX[iCCW][0], ledsX[iCCW][1], ledsX[iCCW][2]);
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

//-m15-R,W,B MARCH CCW
void rwb_march() {
  copy_led_array();
  int iCCW;

  idex++;
  if (idex > 2) { idex = 0; }

  switch (idex) {
    case 0:
      setPixelRGB(0, 255, 0, 0);
      break;
    case 1:
      setPixelRGB(0, 255, 255, 255);
      break;
    case 2:
      setPixelRGB(0, 0, 0, 255);
      break;
  }

  for (int i = 1; i < LED_COUNT; i++ ) {
    iCCW = adjacent_ccw(i);
    setPixelRGB(i, ledsX[iCCW][0], ledsX[iCCW][1], ledsX[iCCW][2]);
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

//-m29-ONE LINE MATRIX
void matrix() {
  copy_led_array();
  int rand = random(0, 100);

  if (rand > 90) {
    setPixelHSV(0, thishue, thissat, 255);
  } else {
    setPixelHSV(0, thishue, thissat, 0);
  }

  for (int i = 1; i < LED_COUNT; i++ ) {
    setPixelRGB(i, ledsX[i - 1][0], ledsX[i - 1][1], ledsX[i - 1][2]);
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

//-m50-MARCH STRIP CW
void strip_march_cw() {
  copy_led_array();
  int iCW;

  for (int i = 0; i < LED_COUNT; i++ ) {
    iCW = adjacent_cw(i);
    setPixelRGB(i, ledsX[iCW][0], ledsX[iCW][1], ledsX[iCW][2]);
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

//-m51-MARCH STRIP CCW
void strip_march_ccw() {
  copy_led_array();
  int iCCW;

  for (int i = 0; i < LED_COUNT; i++ ) {
    iCCW = adjacent_ccw(i);
    setPixelRGB(i, ledsX[iCCW][0], ledsX[iCCW][1], ledsX[iCCW][2]);
  }

  strip.show();
  if (safeDelayWait(thisdelay)) return;
}

#pragma mark - Helpers

void copy_led_array() {
  for (int i = 0; i < LED_COUNT; i++ ) {
    LEDdata led = leds[i];

  #if (COLOR_DEBTH == 1)
    ledsX[i][0] = (led & 0b11000000);
    ledsX[i][1] = ((led & 0b00111000) << 2);
    ledsX[i][2] = ((led & 0b00000111) << 5);
  #elif (COLOR_DEBTH == 2)
    ledsX[i][0] = (((led & 0b1111100000000000) >> 8));
    ledsX[i][1] = (((led & 0b0000011111100000) >> 3));
    ledsX[i][2] = (((led & 0b0000000000011111) << 3));
  #elif (COLOR_DEBTH == 3)	
    ledsX[i][0] = led.r;
    ledsX[i][1] = led.g;
    ledsX[i][2] = led.b;
  #endif // COLOR_DEBTH
  }
}

#endif //IS_ADDITIONAL_FX
