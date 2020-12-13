/*
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver
  Смена выбранных режимов кнопкой. Кнопка подключена на D2 и GND
*/

#define ORDER_GRB               // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 2           // цветовая глубина: 1, 2, 3 (в байтах)

// ВНИМАНИЕ! define настройки (ORDER_GRB и COLOR_DEBTH) делаются до подключения библиотеки!
#include "microLED.h"

#include "GyverButton.h"        // библиотека для работы с кнопками

#define LED_COUNT 211           // число светодиодов в кольце/ленте
#define LED_DT 12               // пин, куда подключен DIN ленты
#define BUTTON_PIN 2            // пин, куда подключена кнопка

#define IS_ADDITIONAL_FX 0      // Флаг, использовать эффеткы из AFX
#define IS_ADAPT_LIGHT 0        // адаптивная подсветка (1 - включить, 0 - выключить)

byte max_bright = 150;          // максимальная яркость (0 - 255)

#define DEMO_FAVS_DELAY 60000 // 1 * 60 * 1000 - 1 minute

byte fav_modes[] = {3, 4, 8, 11, 19, 20, 24, 26, 29, 31};       // список "любимых" режимов
byte num_modes = sizeof(fav_modes);               // получить количество "любимых" режимов (они все по 1 байту..)

unsigned long last_bright;
int new_bright;

volatile byte ledMode = 151;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
  152 - демо-режим
*/

// цвета мячиков для режима
byte ballColors[3][3] = {
  {0xff, 0, 0},
  {0xff, 0xff, 0xff},
  {0   , 0   , 0xff}
};

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;

LEDdata leds[LED_COUNT];        // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, LED_COUNT, LED_DT);  // объект лента

#if IS_ADDITIONAL_FX
  byte ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)
#endif //IS_ADDITIONAL_FX

GButton modeButton(BUTTON_PIN);

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1)
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR

volatile unsigned long waitTime;
volatile byte modeCounter;
volatile boolean breakFlag;
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

void setup() {
  Serial.begin(9600);

  // Настройка ленты
  strip.setBrightness(max_bright);  // ограничить максимальную яркость                    
  strip.clear();
 
  strip.fill(mRGB(0, 0, 0));
  strip.show();

  randomSeed(analogRead(0));

  // Настройка кнопки
  attachInterrupt(0, btnISR, FALLING);
  modeButton.setDebounce(80);      // настройка антидребезга (по умолчанию 80 мс)
  modeButton.setTimeout(750);      // настройка таймаута на удержание (по умолчанию 500 мс)
}

void loop() {
  modeButton.tick();
  
  //Адаптивная яркость
  #if IS_ADAPT_LIGHT
  if (millis() - last_bright > 500) {     // каждые полсекунды
    last_bright = millis();               // сброить таймер
    new_bright = map(analogRead(6), 1, 1000, 3, max_bright);   // считать показания с фоторезистора, перевести диапазон
    strip.setBrightness(new_bright);        // установить новую яркость
  }
  #endif //IS_ADAPT_LIGHT

  //  Работаем с кнопкой
  if (modeButton.isClick()) {
    breakFlag = true;
  }

  if (modeButton.isHolded()) {
    change_mode(1);
  } else if (modeButton.isSingle()) {
    if (++modeCounter >= num_modes) modeCounter = 0;
    changeToFavMode(modeCounter);
  } else if (modeButton.isDouble()) {
    if (--modeCounter == 255) modeCounter = num_modes - 1;
    changeToFavMode(modeCounter);
  } else if (modeButton.isTriple()) {
    change_mode(151);
  }

  // Слушаем серийный порт
  if (Serial.available() > 0) {     // если что-то прислали
    int32_t newMode = Serial.parseInt();
    if (newMode > 0) {
      change_mode(newMode);
    }
  }
  
  switch (ledMode) {
    case  0: break;
    case  1: break;
    case  2: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  3: rainbow_loop(); break;            // крутящаяся радуга
    case  4: random_burst(); break;            // случайная смена цветов
    case  5: color_bounce(); break;            // бегающий светодиод
    case  6: color_bounceFADE(); break;        // бегающий паровозик светодиодов
    case  7: ems_lightsONE(); break;           // вращаются красный и синий
    case  8: ems_lightsALL(); break;           // вращается половина красных и половина синих
    case  9: flicker(); break;                 // случайный стробоскоп
    case 10: pulse_one_color_all(); break;     // пульсация одним цветом
    case 11: pulse_one_color_all_rev(); break; // пульсация со сменой цветов
    case 12: fade_vertical(); break;           // плавная смена яркости по вертикали (для кольца)
    case 13: radiation(); break;               // пульсирует значок радиации
    case 14: color_loop_vardelay(); break;     // красный светодиод бегает по кругу
    case 15: white_temps(); break;             // бело синий градиент (?)
    case 16: sin_bright_wave(); break;         // тоже хрень какая то
    case 17: pop_horizontal(); break;          // красные вспышки спускаются вниз
    case 18: quad_bright_curve(); break;       // полумесяц del
    case 19: flame(); break;                   // эффект пламени (туго отменяется)
    case 20: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)
    case 21: pacman(); break;                  // пакман del
    case 22: random_color_pop(); break;        // безумие случайных вспышек
    case 23: ems_lightsSTROBE(); break;        // полицейская мигалка
    case 24: rgb_propeller(); break;           // RGB пропеллер
    case 25: kitt(); break;                    // случайные вспышки красного в вертикаьной плоскости
    case 26: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга

    case 27: colorWipe(0x00, 0xff, 0x00, thisdelay); 
    colorWipe(0x00, 0x00, 0x00, thisdelay); break;                                  // плавное заполнение цветом
    case 28: CylonBounce(0xff, 0, 0, 4, 10, thisdelay); break;                      // бегающие светодиоды  (долго отменяется)
    case 29: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 30: NewKITT(0xff, 0, 0, 8, 10, thisdelay); break;                          // беготня секторов круга del
    case 31: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга (долго отменяется)
    case 32: TwinkleRandom(20, thisdelay, 1); break;                                // случайные разноцветные включения (1 - танцуют все, 0 - случайный 1 диод)
    case 33: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни (не отменяется)
    case 34: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета
    case 35: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    case 36: theaterChase(0xff, 0, 0, thisdelay); break;                            // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 37: theaterChaseRainbow(thisdelay); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3) (не отменяется)
    case 38: Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000); break;                  // стробоскоп
    case 39: BouncingBalls(0xff, 0, 0, 3); break;                                   // прыгающие мячики
    case 40: BouncingColoredBalls(3, ballColors); break;                            // прыгающие мячики цветные

    // Additinal effects
    #if IS_ADDITIONAL_FX
    case 101: rule30(); break;                  // AFX - безумие красных светодиодов (doesn't work)
    case 102: random_march(); break;            // AFX - безумие случайных цветов
    case 103: rwb_march(); break;               // AFX - белый синий красный бегут по кругу (ПАТРИОТИЗМ!)
    case 104: matrix(); break;                  // AFX - зелёненькие бегают по кругу случайно
    case 105: strip_march_ccw(); break;         // AFX - чёт сломалось
    case 106: strip_march_cw(); break;          // AFX - чёт сломалось
    #endif //IS_ADDITIONAL_FX

    case 151: demo_favs();  break;              // демо из любимых режимов
    case 152: demo_modeA(); break;              // длинное демо
    case 153: demo_modeB(); break;              // короткое демо

    case 201: break;                            //---ALL RED
    case 202: break;                            //---ALL GREEN
    case 203: break;                            //---ALL BLUE
    case 204: break;                            //---ALL COLOR X
    case 205: break;                            //---ALL COLOR Y
    case 206: break;                            //---ALL COLOR Z

    case 255: break;                            // пауза

    default: errorMode(); break;
  }
}

void change_mode(int newmode) {
  Serial.print("New mode is: ");
  Serial.println(newmode);

  thissat = 255;
  
  switch (newmode) {
    case 0: setStripRGB(0, 0, 0); strip.show(); break; //---ALL OFF
    case 1: setStripRGB(255, 255, 255); strip.show(); break; //---ALL ON
    
    case 2: thisdelay = 60; break;                      //---STRIP RAINBOW FADE
    case 3: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 4: thisdelay = 20; break;                      //---RANDOM BURST
    case 5: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    case 6: thisdelay = 80; thishue = 0; break;         //---CYLON v2
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 8: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SOLID
    case 9: thishue = 160; thissat = 50; break;         //---STRIP FLICKER
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 11: thisdelay = 30; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 12: thisdelay = 60; thishue = 180; break;      //---VERTICAL SOMETHING
    case 13: thisdelay = 60; thishue = 95; break;       //---RADIATION SYMBOL
    case 16: thisdelay = 35; thishue = 180; break;      //---SIN WAVE BRIGHTNESS
    case 17: thisdelay = 100; thishue = 0; break;       //---POP LEFT/RIGHT
    case 18: thisdelay = 100; thishue = 180; break;     //---QUADRATIC BRIGHTNESS CURVE
    case 20: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 21: thisdelay = 50; break;                     //---PACMAN
    case 22: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 23: thisdelay = 25; thishue = 0; break;        //---EMERGECNY STROBE
    case 24: thisdelay = 100; thishue = 0; break;       //---RGB PROPELLER
    case 25: thisdelay = 100; thishue = 0; break;       //---KITT
    case 26: thisdelay = 15; break;                     //---NEW RAINBOW LOOP
    case 27: thisdelay = 50; break;                     // colorWipe
    case 28: thisdelay = 50; break;                     // CylonBounce
    case 29: thisdelay = 15; break;                     // Fire
    case 30: thisdelay = 50; break;                     // NewKITT
    case 31: thisdelay = 20; break;                     // rainbowCycle
    case 32: thisdelay = 10; break;                     // rainbowTwinkle
    case 33: thisdelay = 50; break;                     // RunningLights
    case 34: thisdelay = 0; break;                      // Sparkle
    case 35: thisdelay = 30; break;                     // SnowSparkle
    case 36: thisdelay = 50; break;                     // theaterChase
    case 37: thisdelay = 50; break;                     // theaterChaseRainbow
    case 38: thisdelay = 100; break;                    // Strobe

    // Additinal effects
    #if IS_ADDITIONAL_FX
    case 101: thisdelay = 100; break;                   // AFX - CELL AUTO - RULE 30 (RED)
    case 102: thisdelay = 80; break;                    // AFX - MARCH RANDOM COLORS
    case 103: thisdelay = 80; break;                    // AFX - MARCH RWB COLORS
    case 104: thisdelay = 100; thishue = 95; break;     // AFX - зелёненькие бегают по кругу случайно
    case 105: thisdelay = 100; break;                   // AFX - MARCH STRIP NOW CCW
    case 106: thisdelay = 100; break;                   // AFX - MARCH STRIP NOW CCW
    #endif //IS_ADDITIONAL_FX

    case 201: setStripRGB(255, 0, 0);   strip.show(); break; //---ALL RED
    case 202: setStripRGB(0, 255, 0);   strip.show(); break; //---ALL GREEN
    case 203: setStripRGB(0, 0, 255);   strip.show(); break; //---ALL BLUE
    case 204: setStripRGB(255, 255, 0); strip.show(); break; //---ALL COLOR X
    case 205: setStripRGB(0, 255, 255); strip.show(); break; //---ALL COLOR Y
    case 206: setStripRGB(255, 0, 255); strip.show(); break; //---ALL COLOR Z
  }

  bouncedirection = 0;
  setStripRGB(0, 0, 0);
  ledMode = newmode;
}

#pragma mark - Actions

void btnISR() {
  breakFlag = true;
  modeButton.tick();
}

#pragma mark - Helpers

void changeToFavMode(byte newModeNum) {
  modeCounter = newModeNum;
  ledMode = fav_modes[newModeNum];
  change_mode(ledMode);
  breakFlag = true;
}
