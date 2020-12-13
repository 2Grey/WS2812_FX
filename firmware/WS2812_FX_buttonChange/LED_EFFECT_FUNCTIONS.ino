
void errorMode() {
  setStripRGB(255, 0, 0);
  strip.show();
  if (safeDelayWait(1000)) return;

  setStripRGB(0, 0, 0);
  strip.show();
  if (safeDelayWait(1000)) return;
}

bool rainbow_fade() {                         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;

  if (ihue > 255) { ihue = 0; }

  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    setPixelHSV(idex, ihue, thissat, 255);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool rainbow_loop() {                        //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + thisstep;

  if (idex >= LED_COUNT) { idex = 0; }
  if (ihue > 255) { ihue = 0; }

  setPixelHSV(idex, ihue, thissat, 255);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);

  setPixelHSV(idex, ihue, thissat, 255);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool color_bounce() {                        //-m5-BOUNCE COLOR (SINGLE LED)
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == LED_COUNT) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }

  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      setPixelHSV(i, thishue, thissat, 255);
    } else {
      setPixelHSV(i, 0, 0, 0);
    }
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool color_bounceFADE() {                    //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == LED_COUNT) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }

  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }

  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      setPixelHSV(i, thishue, thissat, 255);
    } else if (i == iL1) {
      setPixelHSV(i, thishue, thissat, 150);
    } else if (i == iL2) {
      setPixelHSV(i, thishue, thissat, 80);
    } else if (i == iL3) {
      setPixelHSV(i, thishue, thissat, 20);
    } else if (i == iR1) {
      setPixelHSV(i, thishue, thissat, 150);
    } else if (i == iR2) {
      setPixelHSV(i, thishue, thissat, 80);
    } else if (i == iR3) {
      setPixelHSV(i, thishue, thissat, 20);
    } else {
      setPixelHSV(i, 0, 0, 0);
    }
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool ems_lightsONE() {                    //-m7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= LED_COUNT) { idex = 0; }
  
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishue + 160) % 255;

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idexR) {
      setPixelHSV(i, thishue, thissat, 255);
    } else if (i == idexB) {
      setPixelHSV(i, thathue, thissat, 255);
    } else {
      setPixelHSV(i, 0, 0, 0);
    }
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool ems_lightsALL() {                  //-m8-EMERGENCY LIGHTS (TWO COLOR SOLID)
  idex++;
  if (idex >= LED_COUNT) { idex = 0; }

  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishue + 160) % 255;

  setPixelHSV(idexR, thishue, thissat, 255);
  setPixelHSV(idexB, thathue, thissat, 255);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool flicker() {                          //-m9-FLICKER EFFECT
  int random_bright = random(0, 255);
  int random_delay = random(10, 100);
  int random_bool = random(0, random_bright);

  if (random_bool < 10) {
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      setPixelHSV(i, thishue, thissat, random_bright);
    }

    strip.show();

    if (safeDelayWait(random_delay)) return false;
    return true;
  }
}

bool pulse_one_color_all() {              //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0) {
    ibright++;
    if (ibright >= 255) {
      bouncedirection = 1;
    }
  }

  if (bouncedirection == 1) {
    ibright = ibright - 1;
    if (ibright <= 1) {
      bouncedirection = 0;
    }
  }

  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    setPixelHSV(idex, thishue, thissat, ibright);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool pulse_one_color_all_rev() {           //-m11-PULSE SATURATION ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0) {
    isat++;
    if (isat >= 255) {
      bouncedirection = 1;
    }
  }

  if (bouncedirection == 1) {
    isat = isat - 1;
    if (isat <= 1) {
      bouncedirection = 0;
    }
  }

  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    setPixelHSV(idex, thishue, isat, 255);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool fade_vertical() {                    //-m12-FADE 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) { idex = 0; }

  int idexA = idex;
  int idexB = horizontal_index(idexA);

  ibright = ibright + 10;
  if (ibright > 255) { ibright = 0; }

  setPixelHSV(idexA, thishue, thissat, ibright);
  setPixelHSV(idexB, thishue, thissat, ibright);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool radiation() {                   //-m13-SORT OF RADIATION SYMBOLISH-
  int N3  = int(LED_COUNT / 3);
  int N6  = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);

  for (int i = 0; i < N6; i++ ) {    //-HACKY, I KNOW...
    tcount = tcount + .02;
    if (tcount > 3.14) { tcount = 0.0; }

    ibright = int(sin(tcount) * 255);

    int j0 = (i + LED_COUNT - N12) % LED_COUNT;
    int j1 = (j0 + N3) % LED_COUNT;
    int j2 = (j1 + N3) % LED_COUNT;

    setPixelHSV(j0, thishue, thissat, ibright);
    setPixelHSV(j1, thishue, thissat, ibright);
    setPixelHSV(j2, thishue, thissat, ibright);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool color_loop_vardelay() {                    //-m14-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  idex++;
  if (idex > LED_COUNT) { idex = 0; }

  int di = abs(TOP_INDEX - idex);
  int t = constrain((10 / di) * 10, 10, 500);

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      setPixelHSV(i, 0, thissat, 255);
    } else {
      setPixelRGB(i, 0, 0, 0);
    }
  }

  strip.show();

  if (safeDelayWait(t)) return false;
  return true;
}

bool white_temps() {                            //-m15-SHOW A SAMPLE OF BLACK BODY RADIATION COLOR TEMPERATURES
  int N9 = int(LED_COUNT / 9);

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i >= 0 && i < N9) {
      setPixelRGB(i, 255, 147, 41); //-CANDLE - 1900
    }
    if (i >= N9 && i < N9 * 2) {
      setPixelRGB(i, 255, 197, 143); //-40W TUNG - 2600
    }
    if (i >= N9 * 2 && i < N9 * 3) {
      setPixelRGB(i, 255, 214, 170); //-100W TUNG - 2850
    }
    if (i >= N9 * 3 && i < N9 * 4) {
      setPixelRGB(i, 255, 241, 224); //-HALOGEN - 3200
    }
    if (i >= N9 * 4 && i < N9 * 5) {
      setPixelRGB(i, 255, 250, 244); //-CARBON ARC - 5200
    }
    if (i >= N9 * 5 && i < N9 * 6) {
      setPixelRGB(i, 255, 255, 251); //-HIGH NOON SUN - 5400
    }
    if (i >= N9 * 6 && i < N9 * 7) {
      setPixelRGB(i, 255, 255, 255); //-DIRECT SUN - 6000
    }
    if (i >= N9 * 7 && i < N9 * 8) {
      setPixelRGB(i, 201, 226, 255); //-OVERCAST SKY - 7000
    }
    if (i >= N9 * 8 && i < LED_COUNT) {
      setPixelRGB(i, 64, 156, 255); //-CLEAR BLUE SKY - 20000
    }
  }

  strip.show();

  if (safeDelayWait(100)) return false;
  return true;
}

bool sin_bright_wave() {        //-m16-BRIGHTNESS SINE WAVE
  for (int i = 0; i < LED_COUNT; i++ ) {
    tcount = tcount + .1;
    if (tcount > 3.14) { tcount = 0.0; }

    ibright = int(sin(tcount) * 255);
    setPixelHSV(i, thishue, thissat, ibright);

    strip.show();

    if (safeDelayWait(thisdelay)) return false;
  }

  return true;
}

bool pop_horizontal() {        //-m17-POP FROM LEFT TO RIGHT UP THE RING
  int ix;

  if (bouncedirection == 0) {
    bouncedirection = 1;
    ix = idex;
  } else if (bouncedirection == 1) {
    bouncedirection = 0;
    ix = horizontal_index(idex);
    idex++;
    if (idex > TOP_INDEX) {
      idex = 0;
    }
  }

  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == ix) {
      setPixelHSV(i, thishue, thissat, 255);
    } else {
      setPixelRGB(i, 0, 0, 0);
    }
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool quad_bright_curve() {      //-m18-QUADRATIC BRIGHTNESS CURVER
  int ax;

  for (int x = 0; x < LED_COUNT; x++ ) {
    if (x <= TOP_INDEX) {
      ax = x;
    } else if (x > TOP_INDEX) {
      ax = LED_COUNT - x;
    }

    int a = 1; int b = 1; int c = 0;
    int iquad = -(ax * ax * a) + (ax * b) + c; //-ax2+bx+c
    int hquad = -(TOP_INDEX * TOP_INDEX * a) + (TOP_INDEX * b) + c;
    ibright = int((float(iquad) / float(hquad)) * 255);

    setPixelHSV(x, thishue, thissat, ibright);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool flame() {                                    //-m19-FLAMEISH EFFECT
  int idelay = random(0, 35);
  float hmin = 0.1; 
  float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random(0, 3);
  float hinc = (hdif / float(TOP_INDEX)) + randtemp;
  int ihue = hmin;

  for (int i = 0; i <= TOP_INDEX; i++ ) {
    ihue = ihue + hinc;
    setPixelHSV(i, ihue, thissat, 255);

    int ih = horizontal_index(i);
    setPixelHSV(ih, ihue, thissat, 255);
    setPixelRGB(TOP_INDEX, 255, 255, 255);

    strip.show();

    if (safeDelayWait(idelay)) return false;
  }

  return true;
}

bool rainbow_vertical() {                        //-m20-RAINBOW 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) { idex = 0; }

  ihue = ihue + thisstep;
  if (ihue > 255) { ihue = 0; }

  int idexA = idex;
  int idexB = horizontal_index(idexA);

  setPixelHSV(idexA, ihue, thissat, 255);
  setPixelHSV(idexB, ihue, thissat, 255);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool pacman() {                                  //-m21-REALLY TERRIBLE PACMAN CHOMPING EFFECT
  int s = int(LED_COUNT / 4);
  
  lcount++;
  if (lcount > 5) { lcount = 0; }

  if (lcount == 0) {
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      setPixelRGB(i, 255, 255, 0);
    }
  }

  if (lcount == 1 || lcount == 5) {
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      setPixelRGB(i, 255, 255, 0);
    }
    setPixelRGB(s, 0, 0, 0);
  }

  if (lcount == 2 || lcount == 4) {
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      setPixelRGB(i, 255, 255, 0);
    }

    setPixelRGB(s - 1, 0, 0, 0);
    setPixelRGB(s, 0, 0, 0);
    setPixelRGB(s + 1, 0, 0, 0);
  }

  if (lcount == 3) {
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      setPixelRGB(i, 255, 255, 0);
    }

    setPixelRGB(s - 2, 0, 0, 0);
    setPixelRGB(s - 1, 0, 0, 0);
    setPixelRGB(s, 0, 0, 0);
    setPixelRGB(s + 1, 0, 0, 0);
    setPixelRGB(s + 2, 0, 0, 0);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool random_color_pop() {                         //-m22-RANDOM COLOR POP
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);

  setStripRGB(0, 0, 0);
  setPixelHSV(idex, ihue, thissat, 255);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool ems_lightsSTROBE() {                  //-m23-EMERGENCY LIGHTS (STROBE LEFT/RIGHT)
  int thishue = 0;
  int thathue = (thishue + 160) % 255;

  for (int x = 0 ; x < 5; x++ ) {
    for (int i = 0 ; i < TOP_INDEX; i++ ) {
      setPixelHSV(i, thishue, thissat, 255);
    }

    strip.show(); if (safeDelayWait(thisdelay)) return false;
    setStripHSV(0, 0, 0);
    strip.show(); if (safeDelayWait(thisdelay)) return false;
  }

  for (int x = 0 ; x < 5; x++ ) {
    for (int i = TOP_INDEX ; i < LED_COUNT; i++ ) {
      setPixelHSV(i, thathue, thissat, 255);
    }

    strip.show(); if (safeDelayWait(thisdelay)) return false;
    setStripHSV(0, 0, 0);
    strip.show(); if (safeDelayWait(thisdelay)) return false;
  }

  return true;
}

bool rgb_propeller() {                           //-m24-RGB PROPELLER
  idex++;

  int ghue = (thishue + 80) % 255;
  int bhue = (thishue + 160) % 255;
  int N3  = int(LED_COUNT / 3);
  int N6  = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);

  for (int i = 0; i < N3; i++ ) {
    int j0 = (idex + i + LED_COUNT - N12) % LED_COUNT;
    int j1 = (j0 + N3) % LED_COUNT;
    int j2 = (j1 + N3) % LED_COUNT;

    setPixelHSV(j0, thishue, thissat, 255);
    setPixelHSV(j1, ghue, thissat, 255);
    setPixelHSV(j2, bhue, thissat, 255);
  }

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

bool kitt() {                                     //-m25-KNIGHT INDUSTIES 2000
  int rand = random(0, TOP_INDEX);

  for (int i = 0; i < rand; i++ ) {
    setPixelHSV(TOP_INDEX + i, thishue, thissat, 255);
    setPixelHSV(TOP_INDEX - i, thishue, thissat, 255);

    strip.show();
    if (safeDelayWait(thisdelay / rand)) return false;
  }

  for (int i = rand; i > 0; i-- ) {
    setPixelHSV(TOP_INDEX + i, thishue, thissat, 0);
    setPixelHSV(TOP_INDEX - i, thishue, thissat, 0);

    strip.show();
    if (safeDelayWait(thisdelay / rand)) return false;
  }

  return true;
}

bool new_rainbow_loop() {                      //-m26-RAINBOW FADE FROM FAST_SPI2
  ihue -= 1;

  fill_rainbox(leds, LED_COUNT, ihue, 5);

  strip.show();

  if (safeDelayWait(thisdelay)) return false;
  return true;
}

//-m27-Плавное заполнение цветом
bool colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    setPixelRGB(i, red, green, blue);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return false;
  }

  return true;
}

//-m28-Бегающие светодиоды
bool CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {

  for (int i = 0; i < LED_COUNT - EyeSize - 2; i++) {
    setStripRGB(0, 0, 0);
    strip.show();
    setPixelRGB(i, red / 10, green / 10, blue / 10);

    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }

    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return false;
  }

  if (safeDelayWait(ReturnDelay)) return false;

  for (int i = LED_COUNT - EyeSize - 2; i > 0; i--) {
    setStripRGB(0, 0, 0);
    strip.show();

    setPixelRGB(i, red / 10, green / 10, blue / 10);

    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }
    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return false;
  }

  if (safeDelayWait(ReturnDelay)) return false;
  return true;
}

//-m29-Линейный огонь
bool Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < LED_COUNT; i++) {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = LED_COUNT - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if (random(255) < Sparking) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }

  // Step 4.  Convert heat to LED colors
  for (int j = 0; j < LED_COUNT; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  strip.show();

  if (safeDelayWait(SpeedDelay)) return false;
  return true;
}

//-m31-Очень плавная вращающаяся радуга
bool rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < LED_COUNT; i++) {
      c = Wheel(((i * 256 / LED_COUNT) + j) & 255);
      setPixelRGB(i, *c, *(c + 1), *(c + 2));

      if (safeDelayWait(SpeedDelay)) return false;
    }

    strip.show();
    if (safeDelayWait(SpeedDelay)) return false;
  }

  return true;
}

//-m32-TwinkleRandom
bool TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setStripRGB(0, 0, 0);
  strip.show();

  for (int i = 0; i < Count; i++) {
    setPixelRGB(random(LED_COUNT), random(0, 255), random(0, 255), random(0, 255));
    strip.show();

    if (safeDelayWait(SpeedDelay)) return false;

    if (OnlyOne) {
      setStripRGB(0, 0, 0);
      strip.show();
    }
  }

  if (safeDelayWait(SpeedDelay)) return false;
  return true;
}

//-m33-RunningLights
bool RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int i = 0; i < LED_COUNT * 2; i++) {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < LED_COUNT; i++) {
      setPixelRGB(i, ((sin(i + Position) * 127 + 128) / 255) * red,
                            ((sin(i + Position) * 127 + 128) / 255) * green,
                            ((sin(i + Position) * 127 + 128) / 255) * blue);

      if (safeDelayWait(WaveDelay)) return false;
    }

    strip.show();
    if (safeDelayWait(WaveDelay)) return false;
  }

  return true;
}

//-m34-Sparkle
bool Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(LED_COUNT);

  setPixelRGB(Pixel, red, green, blue);
  strip.show();

  if (safeDelayWait(SpeedDelay)) return false;
  setPixelRGB(Pixel, 0, 0, 0);

  return true;
}

//-m35-SnowSparkle
bool SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setStripRGB(red, green, blue);
  strip.show();

  int Pixel = random(LED_COUNT);
  setPixelRGB(Pixel, 0xff, 0xff, 0xff);
  
  strip.show();
  if (safeDelayWait(SparkleDelay)) return false;

  setPixelRGB(Pixel, red, green, blue);

  strip.show();
  if (safeDelayWait(SpeedDelay)) return false;

  return true;
}

//-m36-TheaterChase
bool theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        setPixelRGB(i + q, red, green, blue); //turn every third pixel on
      }

      strip.show();
      if (safeDelayWait(SpeedDelay)) return false;

      for (int i = 0; i < LED_COUNT; i = i + 3) {
        setPixelRGB(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }

  return true;
}

//-m37-TheaterChaseRainbow
bool theaterChaseRainbow(int SpeedDelay) {
  byte *c;

  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        c = Wheel( (i + j) % 255);
        setPixelRGB(i + q, *c, *(c + 1), *(c + 2)); //turn every third pixel on
      }

      strip.show();
      if (safeDelayWait(SpeedDelay)) return false;

      for (int i = 0; i < LED_COUNT; i = i + 3) {
        setPixelRGB(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }

  return true;
}

//-m38-Strobe
bool Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    setStripRGB(red, green, blue);
    strip.show();

    if (safeDelayWait(FlashDelay)) return false;

    setStripRGB(0, 0, 0);

    strip.show();
    if (safeDelayWait(FlashDelay)) return false;
  }

  if (safeDelayWait(EndPause)) return false;
  return true;
}

//-m39-BouncingBalls
bool BouncingBalls(byte red, byte green, byte blue, int BallCount) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    if (breakFlag) { breakFlag = false; return false; }

    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] = millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce[i] / 1000 , 2.0) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if (Height[i] < 0) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if (ImpactVelocity[i] < 0.01) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }

      Position[i] = round( Height[i] * (LED_COUNT - 1) / StartHeight);
    }

    for (int i = 0 ; i < BallCount ; i++) {
      setPixelRGB(Position[i], red, green, blue);
    }

    strip.show();

    setStripRGB(0, 0, 0);
    strip.show();
  }

  return true;
}

//-------------------------------BouncingColoredBalls---------------------------------------
bool BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt(-2 * Gravity * StartHeight);
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    if (breakFlag) { breakFlag = false; return false; }
    
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] = millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce[i] / 1000 , 2.0) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if (Height[i] < 0) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if (ImpactVelocity[i] < 0.01) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }

      Position[i] = round( Height[i] * (LED_COUNT - 1) / StartHeight);
    }

    for (int i = 0 ; i < BallCount ; i++) {
      setPixelRGB(Position[i], colors[i][0], colors[i][1], colors[i][2]);
    }
    strip.show();

    setStripRGB(0, 0, 0);
    strip.show();
  }

  return true;
}

// --- Demo modes

void demo_favs() {
  waitTime = millis();
  
  for (int i = 0; i < num_modes; i++) {
    thisdelay = DEMO_FAVS_DELAY;
    while (!safeDelayAndBreak(thisdelay)) {
      changeToFavMode(i);
    }
  }
}

void demo_modeA() {
  uint32_t demoDelay = 30000;

  thisdelay = 20; 
  thisstep = 10; 
  thishue = 0; 
  thissat = 255;

  setStripRGB(255, 255, 255); 
  strip.show(); 
  if (safeDelayWait(1200)) return;

  waitTime = millis();

  while(!safeDelayAndBreak(demoDelay)) {
    rainbow_fade();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    rainbow_loop();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    random_burst();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    color_bounce();
  }

  thisdelay = 40;
  while(!safeDelayAndBreak(demoDelay)) {
    color_bounceFADE();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    ems_lightsONE();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    ems_lightsALL();
  }

  thishue = 160; 
  thissat = 50;
  while(!safeDelayAndBreak(demoDelay)) {
    flicker();
  }

  setStripRGB(0, 0, 0); 
  strip.show();

  thisdelay = 15; 
  thishue = 0; 
  thissat = 255;
  while(!safeDelayAndBreak(demoDelay)) {
    pulse_one_color_all();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    pulse_one_color_all_rev();
  }

  thisdelay = 60; 
  thishue = 180;
  while(!safeDelayAndBreak(demoDelay)) {
    fade_vertical();
  }

  setStripRGB(0, 0, 0); ; 
  strip.show();

  thisdelay = 60; 
  thishue = 95;
  while(!safeDelayAndBreak(demoDelay)) {
    radiation();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    color_loop_vardelay();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    white_temps();
  }

  thisdelay = 35; 
  thishue = 180;
  while(!safeDelayAndBreak(demoDelay)) {
    sin_bright_wave();
  }

  thisdelay = 100; 
  thishue = 0;
  while(!safeDelayAndBreak(demoDelay)) {
    pop_horizontal();
  }

  thisdelay = 100; 
  thishue = 180;
  while(!safeDelayAndBreak(demoDelay)) {
    quad_bright_curve();
  }

  setStripRGB(0, 0, 0); 
  strip.show();

  while(!safeDelayAndBreak(demoDelay)) {
    flame();
  }

  thisdelay = 50;
  while(!safeDelayAndBreak(demoDelay)) {
    pacman();
  }

  thisdelay = 50; 
  thisstep = 15;
  while(!safeDelayAndBreak(demoDelay)) {
    rainbow_vertical();
  }

  demo_modeB();

  thisdelay = 5;
  while(!safeDelayAndBreak(demoDelay)) {
    new_rainbow_loop();
  }

  #if IS_ADDITIONAL_FX
  thisdelay = 100;
  while(!safeDelayAndBreak(demoDelay)) {
    rule30();
  }

  thisdelay = 40;
  while(!safeDelayAndBreak(demoDelay)) {
    random_march();
  }

  thisdelay = 80;
  while(!safeDelayAndBreak(demoDelay)) {
    rwb_march();
  }

  thisdelay = 100;
  while(!safeDelayAndBreak(demoDelay)) {
    strip_march_ccw();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    strip_march_cw();
  }
  #endif //IS_ADDITIONAL_FX

  setStripRGB(255, 0, 0);   strip.show(); if (safeDelayWait(1200)) return;
  setStripRGB(0, 255, 0);   strip.show(); if (safeDelayWait(1200)) return;
  setStripRGB(0, 0, 255);   strip.show(); if (safeDelayWait(1200)) return;
  setStripRGB(255, 255, 0); strip.show(); if (safeDelayWait(1200)) return;
  setStripRGB(0, 255, 255); strip.show(); if (safeDelayWait(1200)) return;
  setStripRGB(255, 0, 255); strip.show(); if (safeDelayWait(1200)) return;
}

void demo_modeB() {
  uint32_t demoDelay = DEMO_DELAY;

  setStripRGB(0, 0, 0); 
  strip.show();

  waitTime = millis();
  
  thisdelay = 35;
  while(!safeDelayAndBreak(demoDelay)) {
    random_color_pop();
  }
  while(!safeDelayAndBreak(demoDelay)) {
    ems_lightsSTROBE();
  }

  thisdelay = 50;
  while(!safeDelayAndBreak(demoDelay)) {
    rgb_propeller();
  }

  setStripRGB(0, 0, 0); 
  strip.show();

  thisdelay = 100; 
  thishue = 0;
  while(!safeDelayAndBreak(demoDelay)) {
    kitt();
  }

  setStripRGB(0, 0, 0); 
  strip.show();
  
  #if IS_ADDITIONAL_FX
  thisdelay = 30; 
  thishue = 95;
  while(!safeDelayAndBreak(demoDelay)) {
    matrix();
  }
  
  setStripRGB(0, 0, 0); 
  strip.show();
  #endif //IS_ADDITIONAL_FX
}

//-------------------------------newKITT---------------------------------------
void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = ((LED_COUNT - EyeSize) / 2); i >= 0; i--) {
    setStripRGB(0, 0, 0);
    strip.show();

    setPixelRGB(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }
    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixelRGB(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(LED_COUNT - i - j, red, green, blue);
    }
    setPixelRGB(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return;
  }

  if (safeDelayWait(ReturnDelay)) return;
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i <= ((LED_COUNT - EyeSize) / 2); i++) {
    setStripRGB(0, 0, 0);
    strip.show();

    setPixelRGB(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }
    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixelRGB(LED_COUNT - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(LED_COUNT - i - j, red, green, blue);
    }
    setPixelRGB(LED_COUNT - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return;
  }
  if (safeDelayWait(ReturnDelay)) return;
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = 0; i < LED_COUNT - EyeSize - 2; i++) {
    setStripRGB(0, 0, 0);
    strip.show();

    setPixelRGB(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }
    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return;
  }
  if (safeDelayWait(ReturnDelay)) return;
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for (int i = LED_COUNT - EyeSize - 2; i > 0; i--) {
    setStripRGB(0, 0, 0);
    strip.show();

    setPixelRGB(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixelRGB(i + j, red, green, blue);
    }
    setPixelRGB(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    strip.show();
    if (safeDelayWait(SpeedDelay)) return;
  }
  if (safeDelayWait(ReturnDelay)) return;
}

//-------------------------------newKITT---------------------------------------

#pragma mark - Helpers

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixelRGB(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixelRGB(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixelRGB(Pixel, heatramp, 0, 0);
  }
}
