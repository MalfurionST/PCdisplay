/*
  Проект: PCdisplay
  Версия: 1.6.m3.3
  by MalfurionST

  За основу взят проект "ЭКРАН С ПАРАМЕТРАМИ ЖЕЛЕЗА ПК С РЕОБАСОМ И ПОДСВЕТКОЙ" v. 1.6
  страница проекта https://alexgyver.ru/pcdisplay/, https://github.com/AlexGyver/PCdisplay.
  Добавлено управление адресной светодиодной лентой из проекта "МОДДИНГ ПК С LED И ARDUINO" v. 1.0
  страница проекта https://alexgyver.ru/pcdisplay_v2/, https://github.com/AlexGyver/PCdisplay_v2/.

  Внесены изменения:

  ===== Версия m3.0 =====
  - все настройки перенесены из программы в скетч (есть выбор источника настроек),
  - изменен экран загрузки,
  - изменен второй информационный экран (скорость вентилятора и показания датчиков только если их пины не равны 0),
  - если запрещено получать настройки с ПК то LED и вентилятор работает по температурепературе,
  - добавлено управление яркостью экрана и LED через фоторезистр,
  - переименованы некоторые переменные, добавлены библитеки "GyverTimer" и "GyverButton",
  - удержание любой кнопки 2 сек. переводит на начальный экран,
  - если присвоить значение 0 пинам отвечающим за те или иные функции
    (BTN2, LED_GP_PIN, LED_CP_PIN, LED_R, LED_G, LED_B, FAN_PIN, SENSOR_PIN, BACKLIGHT),
  - то они будут отключены при компиляции скетча,
    добавлена настройка выбора источника данных для адресных светодиодов
  - макс. мин. с компьютера или разные для ЦП и ГП из скетча,
    настройка LED brighness переработана и изменяет яркость дисплея адресной и обычной ленты в пределах
  - минимума и максимума обозначенных в настройках скетча,
  - добавлена настройка для отключения построения графиков (PLOT),
  - добавлена настройка (MICRO) для переключения используемых библиотек (микро версии AlexGyver или стандартные),
  - при использовании стандартной библиотеки датчика DS18B20 используется паразитное питание,
  - при использовании микро нормальное питание и отдельные пины для каждого датчика,
  - если SENSOR_PIN2 = 0 то отображается только один датчик.

  ===== Версия m3.1 =====
  - немного кастомизации (можно выбрать вариант полосы "загрузки").

  ===== Версия m3.2 =====
  - добавлена плавность изменения цвета при режиме по температуре,
  - изменен алгоритм отображения цвета Manual COLOR
    (0 выкл, 1-7 яркие цвета, 8 радуга, 9 огонь, 10-1000 по радуге (с фиолетовым!)).

  ===== Версия m3.3 =====
  - оптимизация памяти,
  - добавлена CRT гамма-коррекция (полином).

  Сравнение использования памяти ядрами:
  1) стандартное ядро
  +--------+-------------------+-------------------+-------------------+-------------------+
  |        | compiler v  7.3.0 | compiler v  8.3.0 | compiler v  9.2.0 | compiler v 10.1.0 |
  | память +----------+--------+----------+--------+----------+--------+----------+--------+
  |        | ст.библ. | micro  | ст.библ. | micro  | ст.библ. | micro  | ст.библ. | micro  |
  +--------+----------+--------+----------+--------+----------+--------+----------+--------+
  | Flash  |    25484 |  22388 |    25400 |  22294 |    27438 |  24222 |    27402 |  24234 |
  | SRAM   |     1932 |   1674 |     1932 |   1674 |     1932 |   1674 |     1932 |   1674 |
  +--------+----------+--------+----------+--------+----------+--------+----------+--------+
  2) GyverCore
  +--------+-------------------+-------------------+-------------------+-------------------+
  |        | compiler v  7.3.0 | compiler v  8.3.0 | compiler v  9.2.0 | compiler v 10.1.0 |
  | память +----------+--------+----------+--------+----------+--------+----------+--------+
  |        | ст.библ. | micro  | ст.библ. | micro  | ст.библ. | micro  | ст.библ. | micro  |
  +--------+----------+--------+----------+--------+----------+--------+----------+--------+
  | Flash  |    25028 |  22552 |    24946 |  22480 |    27022 |  24532 |    26988 |  24526 |
  | SRAM   |     1926 |   1674 |     1926 |   1674 |     1926 |   1674 |     1926 |   1674 |
  +--------+----------+--------+----------+--------+----------+--------+----------+--------+

  Библиотеки:             Версия:  Ссылка github.com:
  FastLED                  3.4.0   https://github.com/FastLED/FastLED
  LiquidCrystal_I2C        1.1.4   https://github.com/marcoschwartz/LiquidCrystal_I2C
  TimerOne                 1.1     https://github.com/PaulStoffregen/TimerOne
  DallasTemperature        3.9.0   https://github.com/milesburton/Arduino-Temperature-Control-Library
  OneWire                  2.3.5   https://github.com/PaulStoffregen/OneWire
  GyverButton              3.7     https://github.com/AlexGyver/GyverLibs
  GyverTimer               3.2     https://github.com/AlexGyver/GyverLibs
  microWire                2.1     https://github.com/AlexGyver/GyverLibs
  microLiquidCrystal_I2C   1.1     https://github.com/AlexGyver/GyverLibs
  microDS18B20             2.2     https://github.com/AlexGyver/GyverLibs

  GyverCore                        https://github.com/AlexGyver/GyverCore

  добавить разрешение датчика при использовании микро
*/

/*
  Что идёт в порт: 0-CPU temp, 1-GPU temp, 2-mother temp, 3-max HDD temp, 4-CPU load, 5-GPU load, 6-RAM use, 7-GPU memory use,
  8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-manualFAN, 13-manualCOLOR, 14-fanCtrl, 15-colorCtrl, 16-brightCtrl, 17-LOGinterval, 18-tempSource
*/

// -------------------------- ПИНЫ ---------------------------
#define LED_R 5          // на красный            (если все три цвета 0 то отключено)
#define LED_G 3          // на зелёный            (если все три цвета 0 то отключено)
#define LED_B 6          // на синий              (если все три цвета 0 то отключено)
#define BTN1 17          // первая кнопка
#define BTN2 16          // вторая кнопка         (если 0 то отключено)
#define BACKLIGHT 11     // пин подсветки дисплея (если 0 то отключено)
#define PHOTO A1         // пин фоторезистора
#define LED_GP_PIN 7     // диоды ГП              (если 0 то отключено)
#define LED_CP_PIN 8     // диоды ЦП              (если 0 то отключено)
#define FAN_PIN 9        // вентиляты             (только 9 или 10 пин, если 0 то отключено)
#define SENSOR_PIN 14    // датчик температуры    (если 0 то отключено) 0
#define SENSOR_PIN2 15   // датчик температуры 2 если используется micro (если 0 то отключено)

// ------------------------ НАСТРОЙКИ ------------------------
#define ERROR_DUTY 90       // скорость вентиляторов при потере связи
#define OPTIONBAR 1         // 1/2/3/4 разные варианты отображения полос
#define ERROR_BACKLIGHT 1   // 0/1 гасить/не гасить подсветку при потере сигнала
#define ERROR_TEMP 1        // 1/0 - показывать/не показывать температуру при потере связи 
#define ERROR_UPTIME 1      // 1/0 - сбрасывать/не сбрасывать uptime при потере связи
#define DISPLAY_ADDR 0x27   // адрес платы дисплея: 0x27 или 0x3f. Если дисплей не работает - смени адрес! На самом дисплее адрес не указан
#define TIME_OUT 3000       // таймаут соединения, мс
#define PLOT 1              // 0/1 не строить/строить графики
#define MICRO 0             // 0/1 micro/стандартные библиотеки

// настройки адресных светодиодов
#define CP_MODE 1       // режим анимации ЦП: 0 - цветная полоса, 1 - огонь
#define GP_MODE 1       // режим анимации ГП: 0 - цветная полоса, 1 - огонь
#define NUM_LEDS 60      // количество светодиодов
#define SOURSE_TEMP 1    // 1 - общие настройки мин. макс. температуры (возможно получение с ПК), 0 - разные указанные ниже
#define CP_MIN_TEMP 35   // мин температура ЦП
#define CP_MAX_TEMP 60   // макс температура ЦП
#define GP_MIN_TEMP 35   // мин температура ГП
#define GP_MAX_TEMP 60   // макс температура ГП

// яркость
#define MIN_PHOTO 40         // мин фоторезистора              (0 - 1023)
#define MAX_PHOTO 700        // макс фоторезистора             (0 - 1023)
#define LCD_BRIGHT_MIN 10    // мин яркость подсветки дисплея  (0 - 255)
#define LCD_BRIGHT_MAX 170   // макс яркость подсветки дисплея (0 - 255)
#define LED_BRIGHT_MIN 127   // мин яркость светодиода (led)   (0 - 255)
#define LED_BRIGHT_MAX 255   // макс яркость светодиода (led)  (0 - 255)
#define MIN_BRIGHTNESS 20    // мин яркость адресной ленты     (0 - 255)
#define MAX_BRIGHTNESS 100   // макс яркость адресной ленты    (0 - 255)

// источник настроек
#define BRIGHT_CONTROL 1   // 0/1 - отключено/включено управление яркостью через фоторезистор (при отключении яркость всегда будет макс.)
#define SOURSE_CONTROL 0   // 0/1 - разрешить/запретить брать настройки с ПК (при запрете берутся настройки ниже)
#define COLOR_CONTROL 0    // 0-1001 - (0 - без подсветки, 1 - белый, 2 - фиолетовый, 3 - синий, 4 - голубй, 5 - зелёный,
//                            6 - жёлтый, 7 - красный, 8 - переливание радугой, 9 - огонь,
//                            10-1000 цвет по радуге (с фиолетовым), 1001 - по температуре)
#define T_MIN 35           // минимальная температура
#define T_MAX 75           // максимальная температура
#define LOG_INTERVAL 10    // интервал обновления графиков (сек.)
#define TSOURCE 2          // 0/1/2/3/4 источник данных для светодиода и вентиляторов (проц., видео., два датчика, максимум проц+видео)
#define FAN_MIN 35         // минимальная скорость вращения вентилятора  (0-100%)
#define FAN_MAX 100        // максимальная скорость вращения вентилятора (0-100%)

// ------------------------ БИБЛИОТЕКИ -----------------------
#if (MICRO)
#include <Wire.h>                             // библиотека для соединения
#else
#include <microWire.h>
#endif
#include <string.h>                           // библиотека расширенной работы со строками
#if (MICRO)
#include <LiquidCrystal_I2C.h>                // библтотека дислея
#else
#include "microLiquidCrystal_I2C.h"
#endif
LiquidCrystal_I2C lcd(DISPLAY_ADDR, 20, 4);   // подключение дисплея
#define printByte(args)  write(args);
#include <GyverTimer.h>                       // библиотека таймера
GTimer brightTimer(MS, 167);                  // настройка таймера проверки освещения
#if (LED_GP_PIN || LED_CP_PIN)
GTimer effTimer(MS, 83);                      // таймер эффектов
#endif
#if (LED_R && LED_G && LED_B)
GTimer effLEDTimer(MS, 167);                  // таймер эффектов
#endif
#include "GyverButton.h"                      // библиотека кнопок
GButton button1(BTN1, LOW_PULL, NORM_OPEN);   // настройка кнопок
#if (BTN2)
GButton button2(BTN2, LOW_PULL, NORM_OPEN);   // настройка кнопок (вторая кнопка)
#endif
#if (LED_GP_PIN && LED_CP_PIN)
#include <FastLED.h>                          // библиотека адресных светодиодов
CRGB leds[NUM_LEDS];                          // диоды ЦП
CRGB leds2[NUM_LEDS];                         // диоды ГП
CRGBPalette16 gPal;
#elif (LED_CP_PIN)
#include <FastLED.h>                          // библиотека адресных светодиодов
CRGB leds[NUM_LEDS];                          // диоды ЦП
CRGBPalette16 gPal;
#elif (LED_GP_PIN)
#include <FastLED.h>                          // библиотека адресных светодиодов
CRGB leds2[NUM_LEDS];                         // диоды ЦП
CRGBPalette16 gPal;
#endif
#if (FAN_PIN)
#include <TimerOne.h>                         // библиотека шим
#endif
#if (SENSOR_PIN && MICRO)
#include <OneWire.h>                          // библиотека протокола датчиков
#include <DallasTemperature.h>                // библиотека датчика
#define TEMPERATURE_PRECISION 9               // точность измерений (9 ... 12)
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1, Thermometer2;
#elif (SENSOR_PIN)
#include <microDS18B20.h>                     // библиотека датчика
MicroDS18B20 sensor1(SENSOR_PIN);
MicroDS18B20 sensor2(SENSOR_PIN2);
#endif

// ------------------ ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ------------------
// знак градуса
byte degree[8] = {0b11100,  0b10100,  0b11100,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};

#if (OPTIONBAR == 1)
// правый край полосы загрузки
byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
// левый край полосы загрузки
byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
// центр полосы загрузки
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
// правый край полосы загрузки заполненный
byte right_full[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
// левый край полосы загрузки заполненный
byte left_full[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
// центр полосы загрузки заполненный
byte center_full[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
#elif (OPTIONBAR == 2)
byte right_empty[8] = {0b11110,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11110};
byte left_empty[8] = {0b01111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b01111};
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
byte right_full[8] = {0b11110,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11110};
byte left_full[8] = {0b01111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b01111};
byte center_full[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
#elif (OPTIONBAR == 3)
byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
byte right_full[8] = {0b11111, 0b00001, 0b11101, 0b11101, 0b11101, 0b11101, 0b00001, 0b11111};
byte left_full[8] = {0b11111, 0b10000, 0b10111, 0b10111, 0b10111, 0b10111, 0b10000, 0b11111};
byte center_full[8] = {0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b11111};
#else
byte right_empty[8] = {0b11110,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11110};
byte left_empty[8] = {0b01111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b01111};
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
byte right_full[8] = {0b11110, 0b00001, 0b11101, 0b11101, 0b11101, 0b11101, 0b00001, 0b11110};
byte left_full[8] = {0b01111, 0b10000, 0b10111, 0b10111, 0b10111, 0b10111, 0b10000, 0b01111};
byte center_full[8] = {0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b11111};
#endif

char inData[82];       // массив входных значений (СИМВОЛЫ)
int PCdata[20];        // массив численных значений показаний с компьютера
byte index = 0;
int display_mode = 6;
String string_convert;
unsigned long timeoutTimer, uptime_timer;
boolean onlineFlag, reDraw_flag = 1, updateDisplay_flag, timeOut_flag = 1;
byte lines[] = {4, 5, 7, 6};
String perc;
unsigned long sec;
unsigned long mins, hrs;
boolean timeOutLCDClear = 0, restoreConnectToPC = 0;
int photo_val;

#if (PLOT)
unsigned long plot_timer;
// блоки для построения графиков
byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};

byte PLOTmem[6][16];   // массив для хранения данных для построения графика (16 значений для 6 параметров)
byte blocks, halfs;
byte plotLines[] = {0, 1, 4, 5, 6, 7}; // 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory

// Названия для легенды графиков
const char plot_0[] = "CPU";
const char plot_1[] = "GPU";
const char plot_2[] = "RAM";
const char plot_3[] = "temp";
const char plot_4[] = "load";
const char plot_5[] = "mem";

// названия ниже должны совпадать с массивами сверху и идти по порядку!
static const char *plotNames0[]  = {
  plot_0, plot_1, plot_0, plot_1, plot_2, plot_1
};
static const char *plotNames1[]  = {
  plot_3, plot_3, plot_4, plot_4, plot_4, plot_5
};
// 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory

#define display_0 0
#else
#define display_0 6
#endif

#if (LED_GP_PIN || LED_CP_PIN)
// полоса
#define COEF 0.2    // плавность движения полосы
// огонь
#define COOLING  55
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
#define FIRE_HEIGHT 20

// переменные адресной ленты
#define INCR_AMOUNT 12
int8_t arcStart, arcEnd = 1;
byte arcSpeed = 1;
boolean arcDir;
//byte arcIncr[] = {3, 10, 5, 4, 3, 3, 2, 2, 1, 1, 1, 1};                      // первоначально
//byte arcIncr[] = {1, 1, 1, 2, 2, 3, 5, 6, 7, 4, 3, 2, 1, 1, 1, 1};           // мой вариант
byte arcIncr[] = {1, 1, 1, 1, 2, 2, 3, 4, 5, 7, 8, 6, 4, 3, 2, 2, 1, 1, 1, 1}; // сумма не более NUM LEDS
byte counter;
byte colorCounter;
float tempCP, loadCP, tempGP, loadGP;
CRGB thisColor;
int thisTemp, thisLoad;
int CP_TEMP_MIN = CP_MIN_TEMP;
int CP_TEMP_MAX = CP_MAX_TEMP;
int GP_TEMP_MIN = GP_MIN_TEMP;
int GP_TEMP_MAX = GP_MAX_TEMP;
#endif

#if (SENSOR_PIN)
boolean updateTemp_flag;
boolean updateTemp_flag2 = 1;
unsigned long T_time;
byte temp1, temp2;
#endif

#if (LED_R || LED_G || LED_B || FAN_PIN)
byte mainTemp;
#endif

#if (LED_R && LED_G && LED_B)
int color = 0;
int R, G, B, LEDcolor;
byte LED = LED_BRIGHT_MAX;
uint32_t prevTime, prevTime2;
byte fireRnd = 0;
float fireValue = 0;
#endif

#if (FAN_PIN)
int duty;
#endif

// -------------------------- СТАРТ --------------------------
void setup() {
  PCdata[16] = 50;
#if (BACKLIGHT)
  pinMode(BACKLIGHT, OUTPUT);
  analogWrite(BACKLIGHT, LCD_BRIGHT_MAX);
#endif
  lcd.init();                        // инициализация дисплея
  lcd.backlight();                   // включитьподсветку дисплея
  lcd.clear();                       // очистить дисплей
  show_logo1();                      // показать логотип1
  lcd.cursor();
  lcd.blink();
  show_logo2();                      // показать логотип2
  Serial.begin(9600);
  button1.setTimeout(2000);
#if (BTN2)
  button2.setTimeout(2000);
#endif
#if (FAN_PIN)
  Timer1.initialize(40);
  Timer1.pwm(FAN_PIN, 400);  // включить вентиляторы на 40%
#endif
#if (LED_GP_PIN)
  FastLED.addLeds<WS2811, LED_GP_PIN, GRB>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif
#if (LED_CP_PIN)
  FastLED.addLeds<WS2811, LED_CP_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif
#if (LED_GP_PIN || LED_CP_PIN)
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
#endif
#if (SENSOR_PIN && MICRO)
  sensors.begin();
  sensors.getAddress(Thermometer1, 0);
  sensors.getAddress(Thermometer2, 1);
  sensors.setResolution(Thermometer1, TEMPERATURE_PRECISION);
  sensors.setResolution(Thermometer2, TEMPERATURE_PRECISION);
#endif
#if (LED_R && LED_G && LED_B)
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  analogWrite(LED_R, 0);
  analogWrite(LED_G, 0);
  analogWrite(LED_B, 0);
#endif
  delay(3000);                      // 3 секунды задержка
  lcd.noBlink();
  lcd.noCursor();
  lcd.clear();                      // очистить дисплей
}

// ---------------------- ОСНОВНОЙ ЦИКЛ ----------------------
void loop() {
  parsing();                // получение данных
#if (SOURSE_CONTROL || SOURSE_TEMP)
  soursedata();             // с ПК или нет
#endif
  timeoutTick();            // проверка таймаута
#if (SENSOR_PIN)
  getTemperature();         // получить значения с датчиков температуры
#endif
  brightcontrol();          // управление яркостью
#if (PLOT)
  updatePlot();             // обновляем массив данных графика
#endif
  buttonsTick();            // опрос кнопок и смена режимов
  updateDisplay();          // обновить показания на дисплее
#if (LED_R || LED_G || LED_B || FAN_PIN)
  SelecMainTemp();          // выбор опорной температуры
#endif
#if (FAN_PIN)
  FANcontrol();             // управление вентиятором
#endif
#if (LED_R && LED_G && LED_B)
  LEDcontrol();             // управление светодиодом
#endif
#if (LED_GP_PIN || LED_CP_PIN)
  ADDRLEDcontrol();         // управление адресными светодиодами
#endif
}

// ------------------- УПРАВЛЕНИЕ ЯРКОСТЬЮ -------------------
void brightcontrol() {
#if (BRIGHT_CONTROL)
  if (brightTimer.isReady()) {
    photo_val += (analogRead(PHOTO) - photo_val) / 10;
#endif
#if (BACKLIGHT && BRIGHT_CONTROL)
    analogWrite(BACKLIGHT, getBright(photo_val, MIN_PHOTO, MAX_PHOTO, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX));

    //    byte lcd_bright = map(photo_val, MIN_PHOTO, MAX_PHOTO, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX);
    //    lcd_bright = constrain(lcd_bright, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX);
    //    analogWrite(BACKLIGHT, lcd_bright);
#endif
#if (LED_R && LED_G && LED_B && BRIGHT_CONTROL)
    LED = getBright(photo_val, MIN_PHOTO, MAX_PHOTO, LED_BRIGHT_MIN, LED_BRIGHT_MAX);

    //    LED = map(photo_val, MIN_PHOTO, MAX_PHOTO, LED_BRIGHT_MIN, LED_BRIGHT_MAX);
    //    LED = constrain(LED, LED_BRIGHT_MIN, LED_BRIGHT_MAX);
#endif
#if (BRIGHT_CONTROL && (LED_GP_PIN || LED_CP_PIN))
    FastLED.setBrightness(getBright(photo_val, MIN_PHOTO, MAX_PHOTO, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

    //    byte thisBrightness = map(photo_val, MIN_PHOTO, MAX_PHOTO, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    //    thisBrightness = constrain(thisBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    //    FastLED.setBrightness(thisBrightness);
#endif
#if (BRIGHT_CONTROL)
  }
#endif

#if (BACKLIGHT && !BRIGHT_CONTROL)
  analogWrite(BACKLIGHT, getBright(PCdata[16], 0, 100, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX));

  //  byte lcd_bright = map(PCdata[16], 0, 100, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX);
  //  lcd_bright = constrain(lcd_bright, LCD_BRIGHT_MIN, LCD_BRIGHT_MAX);
  //  analogWrite(BACKLIGHT, lcd_bright);
#endif
#if (LED_R && LED_G && LED_B && !BRIGHT_CONTROL)
  LED = getBright(PCdata[16], 0, 100, LED_BRIGHT_MIN, LED_BRIGHT_MAX);

  //  LED = map(PCdata[16], 0, 100, LED_BRIGHT_MIN, LED_BRIGHT_MAX);
  //  LED = constrain(LED, LED_BRIGHT_MIN, LED_BRIGHT_MAX);
#endif
#if (LED_GP_PIN && LED_CP_PIN && !BRIGHT_CONTROL)
  FastLED.setBrightness(getBright(PCdata[16], 0, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  //  byte thisBrightness = map(PCdata[16], 0, 100, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  //  thisBrightness = constrain(thisBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  //  FastLED.setBrightness(thisBrightness);
#endif
}

// ---------------------- РАСЧЕТ ЯРКОСТИ ---------------------
byte getBright(byte init_val, byte val_min, byte val_max, byte bright_min, byte bright_max) {
  init_val = map(init_val, val_min, val_max, bright_min, bright_max);
  init_val = constrain(init_val, bright_min, bright_max);
  init_val = (float)0.0044 * init_val * init_val - 0.1589 * init_val + 2.6983;
  return init_val;
}

// --------------------- ПОЛУЧЕНИЕ ДАННЫХ --------------------
void parsing() {
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar != 'E') {
      inData[index] = aChar;
      index++;
      inData[index] = '\0';
    } else {
      char *p = inData;
      char *str;
      index = 0;
      String value = "";
      while ((str = strtok_r(p, ";", &p)) != NULL) {
        string_convert = str;
        PCdata[index] = string_convert.toInt();
        index++;
      }
      index = 0;
      updateDisplay_flag = 1;
#if (SENSOR_PIN)
      updateTemp_flag = 1;
#endif
    }
    timeoutTimer = millis();
    timeOut_flag = 1;
    restoreConnectToPC = 1;
    onlineFlag = 1;
#if (!ERROR_BACKLIGHT)
    lcd.backlight();
#endif
  }
}

// ------------------ ВЫБОР ИСТОЧНИКА ДАННЫХ -----------------
void soursedata() {
#if (SOURSE_CONTROL)
  PCdata[9] = min(FAN_MIN, FAN_MAX);
  PCdata[8] = max(FAN_MIN, FAN_MAX);
  PCdata[11] = min(T_MIN, T_MAX);
  PCdata[10] = max(T_MIN, T_MAX);
  PCdata[17] = LOG_INTERVAL;
  PCdata[18] = TSOURCE;
  PCdata[12] = 0;
  PCdata[16] = 100;
#endif
#if (SOURSE_CONTROL && COLOR_CONTROL == 1001)
  PCdata[13] = 0;
#elif (SOURSE_CONTROL)
  PCdata[13] = 1;
  PCdata[15] = COLOR_CONTROL;
#endif
#if (SOURSE_TEMP && (LED_GP_PIN || LED_CP_PIN))
  CP_TEMP_MIN = PCdata[11];
  CP_TEMP_MAX = PCdata[10];
  GP_TEMP_MIN = PCdata[11];
  GP_TEMP_MAX = PCdata[10];
#endif
}

// -------------- ОБНОВЛЕНИЕ ДАННЫХ ДЛЯ ГАФИКОВ --------------
#if (PLOT)
void updatePlot() {
  if ((millis() - plot_timer) > (PCdata[17] * 1000)) {
    for (int i = 0; i < 6; i++) {           // для каждой строки параметров
      for (int j = 0; j < 15; j++) {        // каждый столбец параметров (кроме последнего)
        PLOTmem[i][j] = PLOTmem[i][j + 1];  // сдвинуть весь массив на шаг ВЛЕВО
      }
    }
    for (int i = 0; i < 6; i++) {
      // запомнить общее число полосок графика в ПОСЛЕДНИЙ элемент массива
      PLOTmem[i][15] = ceil(PCdata[plotLines[i]] / 3);
    }
    plot_timer = millis();
  }
}
#endif

// --------------- ОТСЛЕЖИВАНИЕ НАЖАТИЯ КНОПОК ---------------
void buttonsTick() {
  button1.tick();
#if (BTN2)
  button2.tick();
#endif
  if (button1.isPress()) {
    display_mode++;
    reDraw_flag = 1;
    if (display_mode > 7) display_mode = display_0;
  }
#if (BTN2)
  if (button2.isPress()) {
    display_mode--;
    reDraw_flag = 1;
    if (display_mode < display_0) display_mode = 7;
  }
#endif
  if (button1.isHolded() && display_mode != 6) {
    display_mode = 6;
    reDraw_flag = 1;
  }
#if (BTN2)
  if (button2.isHolded() && display_mode != 6) { // (вторая кнопка)
    display_mode = 6;
    reDraw_flag = 1;
  }
#endif
}

// ------------------- ОБНОВЛЕНИЕ ДИСПЛЕЯ --------------------
void updateDisplay() {
  if (updateDisplay_flag) {
    if (reDraw_flag) {
      lcd.clear();
      switch (display_mode) {
#if (PLOT)
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: draw_plot_symb();
          break;
#endif
        case 6: draw_labels_1();
          break;
        case 7: draw_labels_2();
          break;
      }
      reDraw_flag = 0;
    }
    switch (display_mode) {
#if (PLOT)
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5: draw_plot();
        break;
#endif
      case 6: draw_stats_1();
        break;
      case 7: draw_stats_2();
        break;
      default: debug(); // экран проверки получения данных
    }
    updateDisplay_flag = 0;
  }
  if (timeOutLCDClear)
  {
    reDraw_flag = 1;
  }
}

void draw_stats_1() {
  timeOutLCDClear = 0;
  lcd.setCursor(4, 0); lcd.print(PCdata[0]); lcd.write(223);
  lcd.setCursor(17, 0); lcd.print(PCdata[4]);
  if (PCdata[4] < 10) perc = "% ";
  else if (PCdata[4] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(4, 1); lcd.print(PCdata[1]); lcd.write(223);
  lcd.setCursor(17, 1); lcd.print(PCdata[5]);
  if (PCdata[5] < 10) perc = "% ";
  else if (PCdata[5] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 2); lcd.print(PCdata[7]);
  if (PCdata[7] < 10) perc = "% ";
  else if (PCdata[7] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 3); lcd.print(PCdata[6]);
  if (PCdata[6] < 10) perc = "% ";
  else if (PCdata[6] < 100) perc = "%";
  else perc = "";  lcd.print(perc);

  for (int i = 0; i < 4; i++) {
    byte line = ceil(PCdata[lines[i]] / 10);
    lcd.setCursor(7, i);
    fillBar(line);
  }
}

void draw_stats_2() {
#if (FAN_PIN)
  lcd.setCursor(17, 0); lcd.print(duty);
  if ((duty) < 10) perc = "% ";
  else if ((duty) < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  byte line = ceil(duty / 10);
  lcd.setCursor(7, 0);
  fillBar(line);
#endif
#if (FAN_PIN && SENSOR_PIN && SENSOR_PIN2)
  lcd.setCursor(6, 3); lcd.print(temp1); lcd.write(223);
  lcd.setCursor(16, 3); lcd.print(temp2); lcd.write(223);
#elif (SENSOR_PIN && SENSOR_PIN2)
  lcd.setCursor(6, 0); lcd.print(temp1); lcd.write(223);
  lcd.setCursor(16, 0); lcd.print(temp2); lcd.write(223);
#elif (FAN_PIN && SENSOR_PIN)
  lcd.setCursor(6, 3); lcd.print(temp1); lcd.write(223);
#elif (SENSOR_PIN)
  lcd.setCursor(6, 0); lcd.print(temp1); lcd.write(223);
#endif
  lcd.setCursor(4, 1); lcd.print(PCdata[2]); lcd.write(223);
  lcd.setCursor(16, 1); lcd.print(PCdata[3]); lcd.write(223);
  lcd.setCursor(9, 2);
  sec = (long)(millis() - uptime_timer) / 1000;
  hrs = (float)floor((sec / 3600));
  mins = (float)floor(sec - (hrs * 3600)) / 60;
  sec = sec - (hrs * 3600 + mins * 60);
  if (hrs < 10) lcd.print(0);
  lcd.print(hrs);
  lcd.print(F(":"));
  if (mins < 10) lcd.print(0);
  lcd.print(mins);
  lcd.print(F(":"));
  if (sec < 10) lcd.print(0);
  lcd.print(sec);
}

void draw_labels_1() {
  lcd.createChar(0, degree);
  draw_labels_symb();
  lcd.setCursor(0, 0);
  lcd.print(F("CPU:"));
  lcd.setCursor(0, 1);
  lcd.print(F("GPU:"));
  lcd.setCursor(0, 2);
  lcd.print(F("GPUmem:"));
  lcd.setCursor(0, 3);
  lcd.print(F("RAMuse:"));
}

void draw_labels_2() {
  lcd.createChar(0, degree);
  draw_labels_symb();
#if (FAN_PIN)
  lcd.setCursor(0, 0);
  lcd.print(F("FANsp:"));
#endif
#if (FAN_PIN && SENSOR_PIN && SENSOR_PIN2)
  lcd.setCursor(0, 3);
  lcd.print(F("TMP1:"));
  lcd.setCursor(10, 3);
  lcd.print(F("TMP2:"));
#elif (SENSOR_PIN && SENSOR_PIN2)
  lcd.setCursor(0, 0);
  lcd.print(F("TMP1:"));
  lcd.setCursor(10, 0);
  lcd.print(F("TMP2:"));
#elif (FAN_PIN && SENSOR_PIN)
  lcd.setCursor(0, 3);
  lcd.print(F("TMP1:"));
#elif (SENSOR_PIN)
  lcd.setCursor(0, 0);
  lcd.print(F("TMP1:"));
#endif
  lcd.setCursor(0, 1);
  lcd.print(F("MOM:"));
  lcd.setCursor(9, 1);
  lcd.print(F("HDDmax:"));
  lcd.setCursor(0, 2);
  lcd.print(F("UPTIME:"));
}

// ---------- ЗАПИСЬ СИМВОЛОВ ДЛЯ ЛИНИИ В ДИСПЛЕЙ ------------
void draw_labels_symb() {
  lcd.createChar(1, left_empty);
  lcd.createChar(2, center_empty);
  lcd.createChar(3, right_empty);
  lcd.createChar(4, left_full);
  lcd.createChar(5, center_full);
  lcd.createChar(6, right_full);
}

// ---------------------- ВЫВОД ЛИНИИ ------------------------
void fillBar(byte line) {
  if (line == 0) lcd.printByte(1)
    else lcd.printByte(4);
  for (int n = 1; n < 9; n++) {
    if (n < line) lcd.printByte(5);
    if (n >= line) lcd.printByte(2);
  }
  if (line == 10) lcd.printByte(6)
    else lcd.printByte(3);
}

#if (PLOT)
void draw_legend() {
  byte data = PCdata[plotLines[display_mode]];
  lcd.setCursor(16, 2); lcd.print(data);
  if (display_mode > 1) {
    if (data < 10) perc = "% ";
    else if (data < 100) perc = "%";
    else {
      perc = "";
    }
    lcd.print(perc);
  } else {
    if (data < 10) {
      lcd.write(223);
      lcd.print(F("  "));
    } else if (data < 100) {
      lcd.write(223); lcd.print(F(" "));
    } else {
      lcd.write(223);
    }
  }
}

void draw_plot() {
  draw_legend();
  for (byte i = 0; i < 16; i++) {                  // каждый столбец параметров
    blocks = floor(PLOTmem[display_mode][i] / 8);  // найти количество целых блоков
    halfs = PLOTmem[display_mode][i] - blocks * 8; // найти число оставшихся полосок
    for (byte n = 0; n < 4; n++) {
      if (n < blocks) {
        lcd.setCursor(i, (3 - n));
        lcd.printByte(0);
      }
      if (n >= blocks) {
        if (n != 3) {
          lcd.setCursor(i, (3 - n));
          if (halfs > 0) lcd.printByte(halfs);
          for (byte k = n + 1; k < 4; k++) {
            lcd.setCursor(i, (3 - k));
            lcd.print(F(" "));
          }
          break;
        }
      }
    }
  }
}

// --------- ЗАПИСЬ СИМВОЛОВ ДЛЯ ГРАФИКОВ В ДИСПЛЕЙ ----------
void draw_plot_symb() {
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
  lcd.setCursor(16, 0);
  lcd.print(plotNames0[display_mode]);
  lcd.setCursor(16, 1);
  lcd.print(plotNames1[display_mode]);
}
#endif

// ---------------- ОТЛАДКА ПОЛУЧЕНИЯ ДАННЫХ -----------------
void debug() {
  lcd.clear();
  lcd.setCursor(0, 0);
  for (int j = 0; j < 5; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 1);
  for (int j = 6; j < 10; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 2);
  for (int j = 10; j < 15; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 3);
  for (int j = 15; j < 18; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
}

// -------- ПОЛУЧЕНИЕ ДАННЫХ ОТ ТЕМПЕРАТУРНЫХ ДАТЧИКОВ -------
#if (SENSOR_PIN && MICRO)
void getTemperature() {
  if (updateTemp_flag) {
    sensors.requestTemperatures();
    temp1 = sensors.getTempC(Thermometer1);
    temp2 = sensors.getTempC(Thermometer2);
    updateTemp_flag = 0;
  }
}
#elif (SENSOR_PIN)
void getTemperature() {
  if (updateTemp_flag && updateTemp_flag2) {
    sensor1.requestTemp();
    sensor2.requestTemp();
    T_time = millis();
    updateTemp_flag2 = 0;
  } else if ((millis() - T_time > 1000)) {
    temp1 = sensor1.getTemp();
    temp2 = sensor2.getTemp();
    updateTemp_flag2 = 1;
  }
}
#endif

// ---------------- ВЫБОР ОПОРНОЙ ТЕМПЕРАТУРЫ ----------------
#if (LED_R || LED_G || LED_B || FAN_PIN)
void SelecMainTemp() {
  switch (PCdata[18]) {
    case 0: mainTemp = PCdata[0];                  // взять опорную температуру как CPU
      break;
    case 1: mainTemp = PCdata[1];                  // взять опорную температуру как GPU
      break;
    case 2: mainTemp = max(PCdata[0], PCdata[1]);  // взять опорную температуру как максимум CPU и GPU
      break;
#if (SENSOR_PIN)
    case 3: mainTemp = temp1;
      break;
    case 4: mainTemp = temp2;
      break;
#endif
    default: mainTemp = max(PCdata[0], PCdata[1]); // взять опорную температуру как максимум CPU, GPU
  }
}
#endif

// ----------------- УПРАВЛЕНИЕ ВЕНТИЛЯТОРОМ -----------------
#if (FAN_PIN)
void FANcontrol() {
  if (PCdata[12] == 1)                  // если стоит галочка ManualFAN
    duty = PCdata[14];                  // скважность равна установленной ползунком
  else {                                // если нет
    duty = map(mainTemp, PCdata[11], PCdata[10], PCdata[9], PCdata[8]);
    duty = constrain(duty, PCdata[9], PCdata[8]);
  }
  Timer1.pwm(FAN_PIN, duty * 10);
}
#endif

// ------------------ УПРАВЛЕНИЕ СВЕТОДИОДОМ -----------------
#if (LED_R && LED_G && LED_B)
void LEDcontrol() {
  if (PCdata[13] == 1) {       // если стоит галочка Manual Color
    LEDcolor = PCdata[15];                         // цвет равен установленному ползунком
    switch (PCdata[15]) {
      case 0: {
          R = 0;
          G = 0;
          B = 0;
        }
        break;
      case 1: {
          R = 255;
          G = 255;
          B = 255;
        }
        break;
      case 2: {
          R = 255;
          G = 0;
          B = 255;
        }
        break;
      case 3: {
          R = 0;
          G = 0;
          B = 255;
        }
        break;
      case 4: {
          R = 0;
          G = 255;
          B = 255;
        }
        break;
      case 5: {
          R = 0;
          G = 255;
          B = 0;
        }
        break;
      case 6: {
          R = 255;
          G = 255;
          B = 0;
        }
        break;
      case 7: {
          R = 255;
          G = 0;
          B = 0;
        }
        break;
      case 8: if (effLEDTimer.isReady()) {
          if (color <= 255) {                       // красный макс, зелёный растёт
            R = 255;
            G = color;
            B = 0;
          }
          else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
            R = 510 - color;
            G = 255;
            B = 0;
          }
          else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
            R = 0;
            G = 255;
            B = color - 510;
          }
          else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
            R = 0;
            G = 1020 - color;
            B = 255;
          }
          else if (color > 1020 && color <= 1275) {   // синий макс, растёт красный
            R = color - 1020;
            G = 0;
            B = 255;
          }
          else if (color > 1275 && color <= 1530) { // красный макс, падает синий
            R = 255;
            G = 0;
            B = 1530 - color;
          }
          color += 10;
          if (color > 1530) color = 0;
        }
        break;
      case 9: {
          // задаём направление движения огня
          if (millis() - prevTime > 100) {
            prevTime = millis();
            fireRnd = random(0, 10);
          }
          // двигаем пламя
          if (millis() - prevTime2 > 20) {
            prevTime2 = millis();
            fireValue = (float)fireValue * (1 - 0.15) + (float)fireRnd * 10 * 0.15;
            uint8_t h = constrain(map(fireValue, 20, 60, 0, 10), 0, 255);
            float V = constrain(map(fireValue, 20, 60, 80, 255), 0, 255);
            float H = (float)h / 255;
            int s = int(H * 6);
            float f = H * 6 - s;
            int q = V * (1 - f);
            int t = V * f;
            switch (s % 6) {
              case 0: R = V, G = t, B = 0; break;
              case 1: R = q, G = V, B = 0; break;
              case 2: R = 0, G = V, B = t; break;
              case 3: R = 0, G = q, B = V; break;
              case 4: R = t, G = 0, B = V; break;
              case 5: R = V, G = 0, B = q; break;
            }
          }
        }
        break;
      default: {
          color = map(PCdata[15], 10, 1000, 0, 1530);
          color = constrain(color, 0, 1530);
          if (color <= 255) {                       // красный макс, зелёный растёт
            R = 255;
            G = color;
            B = 0;
          }
          else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
            R = 510 - color;
            G = 255;
            B = 0;
          }
          else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
            R = 0;
            G = 255;
            B = color - 510;
          }
          else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
            R = 0;
            G = 1020 - color;
            B = 255;
          }
          else if (color > 1020 && color <= 1275) {   // синий макс, растёт красный
            R = color - 1020;
            G = 0;
            B = 255;
          }
          else if (color > 1275 && color <= 1530) { // красный макс, падает синий
            R = 255;
            G = 0;
            B = 1530 - color;
          }
        };
    }
  } else {
    if (effLEDTimer.isReady()) {              // если нет
      int LEDcolor_val = map(mainTemp, PCdata[11], PCdata[10], 0, 1000);
      LEDcolor_val = constrain(LEDcolor_val, 0, 1000);
      LEDcolor += (LEDcolor_val - LEDcolor) / 10;    // плавное изменение цвета
      // синий максимум, плавно прибавляется зелёный
      // зелёный максимум, плавно убавляется синий
      // зелёный максимум, плавно прибавляется красный
      // красный максимум, плавно убавляется зелёный
      if (LEDcolor <= 250) {
        R = 0;
        G = map(LEDcolor, 0, 250, 0, 255);
        B = 255;
      }
      if (LEDcolor > 250 && LEDcolor <= 500) {
        R = 0;
        G = 255;
        B = 255 - map(LEDcolor, 250, 500, 0, 255);
      }
      if (LEDcolor > 500 && LEDcolor <= 750) {
        R = map(LEDcolor, 500, 750, 0, 255);
        G = 255;
        B = 0;
      }
      if (LEDcolor > 750 && LEDcolor <= 1000) {
        R = 255;
        G = 255 - map(LEDcolor, 750, 1000, 0, 255);
        B = 0;
      }
    }
  }
  analogWrite(LED_R, LED * R / 255);
  analogWrite(LED_G, LED * G / 255);
  analogWrite(LED_B, LED * B / 255);
}
#endif

// -------------------- ПРОВЕРКА ТАЙМАУТА --------------------
void timeoutTick() {
  // если связь была потеряна, но восстановилась сбросить uptime, если разрешено
  if (!restoreConnectToPC && ERROR_UPTIME) uptime_timer = millis();
  if (onlineFlag && (millis() - timeoutTimer > TIME_OUT)) {
    lcd.clear();
    onlineFlag = 0;
  }
  if (!onlineFlag) {
    while (Serial.available() < 1) {
      if ((millis() - timeoutTimer > TIME_OUT) && timeOut_flag) {
        index = 0;
        if (restoreConnectToPC) {
          reDraw_flag = 1;
          restoreConnectToPC = 0;
          if (reDraw_flag) {
            lcd.clear();
            reDraw_flag = 0;
          }
        }
        lcd.setCursor(5, 1);
        lcd.print(F("CONNECTION"));
        lcd.setCursor(7, 2);
        lcd.print(F("FAILED"));
        brightcontrol();
        reDraw_flag = 0;
        updateDisplay_flag = 1;
        timeOutLCDClear = 1;
        if (timeOutLCDClear) {
          reDraw_flag = 1;
        }
        display_mode = 6;
#if (ERROR_TEMP && SENSOR_PIN)
        getTemperature();
        updateTemp_flag = 1;
#endif
#if (ERROR_TEMP && SENSOR_PIN && SENSOR_PIN2)
        lcd.setCursor(0, 0);
        lcd.print(F("TMP1:"));
        lcd.setCursor(10, 0);
        lcd.print(F("TMP2:"));
        lcd.setCursor(5, 0); lcd.print(temp1); lcd.write(223);
        lcd.setCursor(15, 0); lcd.print(temp2); lcd.write(223);
#elif (ERROR_TEMP && SENSOR_PIN)
        lcd.setCursor(0, 0);
        lcd.print(F("TMP1:"));
        lcd.setCursor(5, 0); lcd.print(temp1); lcd.write(223);
#endif
#if (FAN_PIN)
        Timer1.pwm(FAN_PIN, ERROR_DUTY * 10);
#endif
#if (ERROR_BACKLIGHT && (LED_GP_PIN || LED_CP_PIN))
        if (effTimer.isReady()) {
          FastLED.clear();
          loading();
          FastLED.show();
        }
#elif  (!ERROR_BACKLIGHT && (LED_GP_PIN || LED_CP_PIN))
        FastLED.clear();
        FastLED.show();
#endif
#if (!ERROR_BACKLIGHT)
        lcd.noBacklight();
#endif
#if (LED_R && LED_G && LED_B && ERROR_BACKLIGHT)
        analogWrite(LED_R, LED * R / 255);
        analogWrite(LED_G, LED * G / 255);
        analogWrite(LED_B, LED * B / 255);
#elif (LED_R && LED_G && LED_B && !ERROR_BACKLIGHT)
        analogWrite(LED_R, 0);
        analogWrite(LED_G, 0);
        analogWrite(LED_B, 0);
#endif
      }
    }
  }
}

#if (LED_GP_PIN || LED_CP_PIN)
void ADDRLEDcontrol() {
  if (effTimer.isReady()) { // управление адресными светодиодами
    FastLED.clear();
#endif
#if (CP_MODE && LED_CP_PIN)
    fireCP();
#elif (LED_CP_PIN)
    lineCP();
#endif
#if (GP_MODE && LED_GP_PIN)
    fireGP();
#elif (LED_GP_PIN)
    lineGP();
#endif
#if (LED_GP_PIN || LED_CP_PIN)
    FastLED.show();
  }
}
#endif

// --------------------- ЦВЕТОВАЯ ПОЛОСА ---------------------
#if (!CP_MODE && LED_CP_PIN)
void lineCP() {
  // переводим температуру от мин-макс в диапазон 0-(половина кольца)
  int thisTemp = map(PCdata[0], CP_TEMP_MIN, CP_TEMP_MAX, 0, NUM_LEDS / 2);
  // ограничиваем диапазон
  thisTemp = constrain(thisTemp, 0, NUM_LEDS / 2);
  // фильтруем running average
  tempCP += (thisTemp - tempCP) * COEF;
  // заливаем дугу
  fillArc(0, ceil(tempCP), CHSV(map((int)tempCP, 0, NUM_LEDS / 2, 65, 0), 255, 255), leds);
  // переводим загрузку от 0-100 в диапазон 0-(половина кольца)
  int thisLoad = map(PCdata[4], 0, 100, 0, NUM_LEDS / 2);
  // фильтруем
  loadCP += (thisLoad - loadCP) * COEF;
  // заливаем дугу
  fillArc(NUM_LEDS - 1 - (int)loadCP, NUM_LEDS, CHSV(map((int)loadCP, 0, NUM_LEDS / 2, 85, 140), 255, 255), leds);
}
#endif

#if (!GP_MODE && LED_GP_PIN)
void lineGP() {
  // тут тож самое, только для ГП
  int thisTemp = map(PCdata[1], CP_TEMP_MIN, CP_TEMP_MAX, 0, NUM_LEDS / 2);
  thisTemp = constrain(thisTemp, 0, NUM_LEDS / 2);
  tempGP += (thisTemp - tempGP) * COEF;
  fillArc(0, ceil(tempGP), CHSV(map((int)tempGP, 0, NUM_LEDS / 2, 65, 0), 255, 255), leds2);
  int thisLoad = map(PCdata[5], 0, 100, 0, NUM_LEDS / 2);
  loadGP += (thisLoad - loadGP) * COEF;
  fillArc(NUM_LEDS - 1 - (int)loadGP, NUM_LEDS, CHSV(map((int)loadGP, 0, NUM_LEDS / 2, 85, 140), 255, 255), leds2);
}
#endif

// --------------------------- ОГОНЬ -------------------------
#if (CP_MODE && LED_CP_PIN)
void fireCP() {
  random16_add_entropy(random());
  // расчёт длины полосы от температуры ЦП
  int thisTemp = map(PCdata[0], CP_TEMP_MIN, CP_TEMP_MAX, 0, NUM_LEDS / 2);
  thisTemp = constrain(thisTemp, 0, NUM_LEDS / 2);
  FireRED_CP(thisTemp);
  // расчёт длины полосы от нагрузки ЦП
  FireBLUE_CP(map(PCdata[4], 0, 100, 0, NUM_LEDS / 2));
}

void FireBLUE_CP(byte height) {
  byte startPos = NUM_LEDS - height;
  gPal = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < height; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / height) + 2));
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = height - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < height; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    pixelnumber = (height - 1) - j;
    leds[startPos + pixelnumber] = color; // в обратном направлении начиная с конца пламени
  }
}

void FireRED_CP(byte height) {
  gPal = HeatColors_p;
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < height; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / height) + 2));
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = height - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < height; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    leds[j] = color;      // в прямом направлении начиная с 0 (начала ленты)
  }
}
#endif

#if (GP_MODE && LED_GP_PIN)
void fireGP() {
  random16_add_entropy(random());
  // расчёт длины полосы от температуры ГП
  int thisTemp = map(PCdata[1], GP_TEMP_MIN, GP_TEMP_MAX, 0, NUM_LEDS / 2);
  thisTemp = constrain(thisTemp, 0, NUM_LEDS / 2);
  FireRED_GP(thisTemp);
  // расчёт длины полосы от нагрузки ГП
  FireBLUE_GP(map(PCdata[5], 0, 100, 0, NUM_LEDS / 2));
}

void FireBLUE_GP(byte height) {
  byte startPos = NUM_LEDS - height;
  gPal = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < height; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / height) + 2));
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = height - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < height; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    pixelnumber = (height - 1) - j;
    leds2[startPos + pixelnumber] = color;
  }
}

void FireRED_GP(byte height) {
  gPal = HeatColors_p;
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < height; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / height) + 2));
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = height - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255) );
  }
  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < height; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    leds2[j] = color;
  }
}
#endif

// ------------------------- ЗАГРУЗКА ------------------------
#if (LED_GP_PIN || LED_CP_PIN)
void loading() {
  if (!arcDir) {
    arcStart += arcSpeed;
    arcEnd += arcIncr[counter] + arcSpeed;
  } else {
    arcStart += arcIncr[counter] + arcSpeed;
    arcEnd += arcSpeed;
  }
  if (arcStart >= NUM_LEDS) arcStart = (arcStart - NUM_LEDS);
  if (arcEnd >= NUM_LEDS) arcEnd = (arcEnd - NUM_LEDS);

  if (++counter >= INCR_AMOUNT) {
    counter = 0;
    arcDir = !arcDir;
    if (!arcDir) {
      colorCounter += 70;
      thisColor = CHSV(colorCounter, 255, 255);
    }
  }
#endif
#if (LED_GP_PIN && LED_CP_PIN)
  if (arcStart < arcEnd) {
    fillArc(arcStart, arcEnd, thisColor, leds);
    fillArc(arcStart, arcEnd, thisColor, leds2);
  } else {
    fillArc(arcStart, NUM_LEDS, thisColor, leds);
    fillArc(arcStart, NUM_LEDS, thisColor, leds2);
    fillArc(0, arcEnd, thisColor, leds);
    fillArc(0, arcEnd, thisColor, leds2);
  }
}
#elif (LED_CP_PIN)
  if (arcStart < arcEnd) {
    fillArc(arcStart, arcEnd, thisColor, leds);
  } else {
    fillArc(arcStart, NUM_LEDS, thisColor, leds);
    fillArc(0, arcEnd, thisColor, leds);
  }
}
#elif (LED_GP_PIN)
  if (arcStart < arcEnd) {
    fillArc(arcStart, arcEnd, thisColor, leds2);
  } else {
    fillArc(arcStart, NUM_LEDS, thisColor, leds2);
    fillArc(0, arcEnd, thisColor, leds2);
  }
}
#endif

// ------------------- ВЫВОД НА СВЕТОДИОДЫ -------------------
#if (LED_GP_PIN || LED_CP_PIN)
void fillArc(byte startPos, byte endPos, CRGB color, CRGB *leds) {
  for (byte i = startPos; i < endPos; i++) {
    leds[i] = color;
  }
}
#endif

// ------------------------ ЛОГОТИП 1 ------------------------
void show_logo1() {
  byte logo[8] = {0b00000, 0b00001,  0b00011,  0b00110,  0b01100,  0b01100,  0b11000,  0b11000};
  lcd.createChar(0, logo);
  logo[1] = 0b10000;
  logo[2] = 0b11000;
  logo[3] = 0b01100;
  logo[4] = 0b00110;
  logo[5] = 0b00110;
  logo[6] = 0b00011;
  logo[7] = 0b00011;
  lcd.createChar(1, logo);
  logo[0] = 0b11000;
  logo[1] = 0b11000;
  logo[2] = 0b01100;
  logo[3] = 0b01100;
  logo[4] = 0b00110;
  logo[5] = 0b00011;
  logo[6] = 0b00001;
  logo[7] = 0b00000;
  lcd.createChar(2, logo);
  logo[0] = 0b00011;
  logo[1] = 0b00011;
  logo[2] = 0b00110;
  logo[3] = 0b00110;
  logo[4] = 0b01100;
  logo[5] = 0b11000;
  logo[6] = 0b10000;
  lcd.createChar(3, logo);
  logo[0] = 0b11111;
  logo[1] = 0b11111;
  logo[2] = 0b00000;
  logo[3] = 0b00000;
  logo[4] = 0b00000;
  logo[5] = 0b00000;
  logo[6] = 0b00000;
  logo[7] = 0b11111;
  lcd.createChar(4, logo);
  logo[7] = 0b00100;
  lcd.createChar(5, logo);
  logo[1] = 0b00100;
  logo[2] = 0b00100;
  logo[6] = 0b11111;
  logo[7] = 0b11111;
  lcd.createChar(7, logo);
  logo[0] = 0b00000;
  logo[1] = 0b00000;
  logo[2] = 0b00000;
  lcd.createChar(6, logo);

  lcd.setCursor(13, 1);
  lcd.printByte(0);
  lcd.printByte(4);
  lcd.printByte(1);
  lcd.printByte(0);
  lcd.printByte(5);
  lcd.printByte(1);
  lcd.setCursor(13, 2);
  lcd.printByte(2);
  lcd.printByte(6);
  lcd.printByte(3);
  lcd.printByte(2);
  lcd.printByte(7);
  lcd.printByte(3);
}

// ------------------------ ЛОГОТИП 2 ------------------------
void show_logo2() {
  char logo[] = "PCdisplay v1.6.m3.3"; // массив логотипа
  for (int i = 0; i < 9; i++) {        // показать логотип первая строка
    lcd.setCursor(i + 1, 1);
    delay(200);
    lcd.print(logo[i]);
  }
  for (int i = 10; i < 19; i++) {      // показать логотип вторая строка
    lcd.setCursor(i - 9, 2);
    delay(200);
    lcd.print(logo[i]);
  }
}
