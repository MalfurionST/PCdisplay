# Экран параметров железа ПК
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Используемые библиотеки](#chapter-2)
* [Настройки в коде](#chapter-3)
* [Схема подключения](#chapter-4)
* [Анализ использования памяти](#chapter-5)

### Версии прошивки
- m3.0 (первая которую решил выложить)
  - все настройки перенесены из программы в скетч (есть выбор источника настроек)
  - если запрещено получать настройки с ПК то LED и вентилятор работает по температурепературе
  - добавлено управление яркостью экрана и LED через фоторезистр
  - добавлены библитеки "GyverTimer" и "GyverButton"
  - удержание любой кнопки 2 сек. переводит на начальный экран
  - если присвоить значение 0 пинам отвечающим за те или иные функции (BTN2, LED_GP_PIN, LED_CP_PIN, LED_R, LED_G, LED_B, FAN_PIN, SENSOR_PIN, BACKLIGHT), то они будут отключены при компиляции скетча
  - добавлена настройка выбора источника данных для адресных светодиодов макс. мин. с компьютера или разные для ЦП и ГП из скетча
  - настройка LED brighness переработана и изменяет яркость дисплея адресной и обычной ленты в пределах минимума и максимума обозначенных в настройках скетча
  - добавлена настройка для отключения построения графиков (PLOT)
  - добавлена настройка (MICRO) для переключения используемых библиотек (микро версии AlexGyver или стандартные)
  - при использовании стандартной библиотеки датчика DS18B20 используется паразитное питание, при использовании микро нормальное питание и отдельные пины для каждого датчика
  - если SENSOR_PIN2 = 0 то отображается только один датчик
- m3.1
  - немного кастомизации (можно выбрать вариант полосы "загрузки")
- m3.2
  - добавлена плавность изменения цвета при режиме по температуре
  - изменен алгоритм отображения цвета Manual COLOR (0 выкл, 1-7 яркие цвета, 8 радуга, 9 огонь, 10-1000 по радуге (с фиолетовым!))

<a id="chapter-0"></a>
## Описание проекта
За основу взяты проекты:
  - ["ЭКРАН С ПАРАМЕТРАМИ ЖЕЛЕЗА ПК С РЕОБАСОМ И ПОДСВЕТКОЙ"](https://alexgyver.ru/pcdisplay/) v. 1.6
  - ["МОДДИНГ ПК С LED И ARDUINO"](https://alexgyver.ru/pcdisplay_v2/) v. 1.0

<a id="chapter-1"></a>
## Папки проекта
- **libraries** - библиотеки проекта. Заменить имеющиеся версии
- **firmware** - прошивка для Arduino, файл в папке открыть в Arduino IDE
- **HardwareMonitor** - программа, необходимая для работы устройства
- **schemes** - наглядная схема

<a id="chapter-2"></a>
## Используемые библиотеки
* [FastLED](https://github.com/FastLED/FastLED) v. 3.3.3
* [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C) v. 1.1.4
* [TimerOne](https://github.com/PaulStoffregen/TimerOne) v. 1.1
* [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) v. 3.9.0
* [OneWire](https://github.com/PaulStoffregen/OneWire) v. 2.3.5
* [GyverButton](https://github.com/AlexGyver/GyverLibs) v. 3.5
* [GyverTimer](https://github.com/AlexGyver/GyverLibs) v. 3.2
* [microWire](https://github.com/AlexGyver/GyverLibs) v. 2.1
* [microLiquidCrystal_I2C](https://github.com/AlexGyver/GyverLibs) v. 1.1
* [microDS18B20](https://github.com/AlexGyver/GyverLibs) v. 2.2

<a id="chapter-3"></a>
## Настройки в коде

    -------------------------- ПИНЫ ---------------------------
    LED_R 5          // на красный            (если все три цвета 0 то отключено)
    LED_G 3          // на зелёный            (если все три цвета 0 то отключено)
    LED_B 6          // на синий              (если все три цвета 0 то отключено)
    BTN1 17          // первая кнопка
    BTN2 16          // вторая кнопка         (если 0 то отключено)                      
    BACKLIGHT 11     // пин подсветки дисплея (если 0 то отключено)
    PHOTO A1         // пин фоторезистора
    LED_GP_PIN 7     // диоды ГП              (если 0 то отключено)
    LED_CP_PIN 8     // диоды ЦП              (если 0 то отключено)
    FAN_PIN 9        // вентиляты             (только 9 или 10 пин, если 0 то отключено)
    SENSOR_PIN 14    // датчик температуры    (если 0 то отключено)
    SENSOR_PIN2 15   // датчик температуры 2 если используется micro (если 0 то отключено)

    ------------------------ НАСТРОЙКИ ------------------------
    ERROR_DUTY 90       // скорость вентиляторов при потере связи
    OPTIONBAR 1         // 1/2/3/4 разные варианты отображения полос
    ERROR_BACKLIGHT 1   // 0/1 гасить/не гасить подсветку при потере сигнала
    ERROR_TEMP 1        // 1/0 - показывать/не показывать температуру при потере связи 
    ERROR_UPTIME 1      // 1/0 - сбрасывать/не сбрасывать uptime при потере связи
    DISPLAY_ADDR 0x27   // адрес платы дисплея: 0x27 или 0x3f. Если дисплей не работает - смени адрес! На самом дисплее адрес не указан
    TIME_OUT 3000       // таймаут соединения, мс
    PLOT 1              // 0/1 не строить/строить графики
    MICRO 0             // 0/1 micro/стандартные библиотеки

    настройки адресных светодиодов
    CP_MODE 1       // режим анимации ЦП: 0 - цветная полоса, 1 - огонь
    GP_MODE 1       // режим анимации ГП: 0 - цветная полоса, 1 - огонь
    NUM_LEDS 60      // количество светодиодов
    SOURSE_TEMP 1    // 1 - общие настройки мин. макс. температуры (возможно получение с ПК), 0 - разные указанные ниже
    CP_MIN_TEMP 35   // мин температура ЦП
    CP_MAX_TEMP 60   // макс температура ЦП
    GP_MIN_TEMP 35   // мин температура ГП
    GP_MAX_TEMP 60   // макс температура ГП

    яркость
    MIN_PHOTO 40         // мин фоторезистора              (0 - 1023)
    MAX_PHOTO 700        // макс фоторезистора             (0 - 1023)
    LCD_BRIGHT_MIN 10    // мин яркость подсветки дисплея  (0 - 255)
    LCD_BRIGHT_MAX 170   // макс яркость подсветки дисплея (0 - 255)
    LED_BRIGHT_MIN 127   // мин яркость светодиода (led)   (0 - 255)
    LED_BRIGHT_MAX 255   // макс яркость светодиода (led)  (0 - 255)
    MIN_BRIGHTNESS 20    // мин яркость адресной ленты     (0 - 255)
    MAX_BRIGHTNESS 100   // макс яркость адресной ленты    (0 - 255)

    источник настроек
    BRIGHT_CONTROL 1   // 0/1 - отключено/включено управление яркостью через фоторезистор (при отключении яркость всегда будет макс.)
    SOURSE_CONTROL 0   // 0/1 - разрешить/запретить брать настройки с ПК (при запрете берутся настройки ниже)
    COLOR_CONTROL 0    // 0-1001 - (0 - без подсветки, 1 - белый, 2 - фиолетовый, 3 - синий, 4 - голубй, 5 - зелёный,
                          6 - жёлтый, 7 - красный, 8 - переливание радугой, 9 - огонь, 
                          10-1000 цвет по радуге (с фиолетовым), 1001 - по температуре)
    T_MIN 35           // минимальная температура
    T_MAX 75           // максимальная температура
    LOG_INTERVAL 10    // интервал обновления графиков (сек.)
    TSOURCE 2          // 0/1/2/3/4 источник данных для светодиода и вентиляторов (проц., видео., два датчика, максимум проц+видео)
    FAN_MIN 35         // минимальная скорость вращения вентилятора  (0-100%)
    FAN_MAX 100        // максимальная скорость вращения вентилятора (0-100%)

<a id="chapter-4"></a>
## Схема подключения
* Обязательные компоненты:
- ардуино, дисплей 2004

![SCHEME](https://github.com/MalfurionST/PCdisplay/blob/master/schemes/PCdisplay.png)

<a id="chapter-5"></a>
## Анализ использования памяти
* Стандартное ядро (стандартные / miсro библиотеки)

память  | compiler v  7.3.0 | compiler v  8.3.0 | compiler v  9.2.0 | compiler v 10.1.0 |
--------|-------------------|-------------------|-------------------|-------------------|
Flash   | 25778 / 22682     | 25770 / 22666     | 27834 / 24578     | 27794 / 24576     |
SRAM    | 1937 / 1680       | 1937 / 1680       | 1937 / 1680       | 1937 / 1680       |

* GyverCore (стандартные / miсro библиотеки)

память  | compiler v  7.3.0 | compiler v  8.3.0 | compiler v  9.2.0 | compiler v 10.1.0 |
--------|-------------------|-------------------|-------------------|-------------------|
Flash   | 25400 / 22890     | 25376 / 22896     | 27464 / 24936     | 27436 / 24904     |
SRAM    | 1931 / 1680       | 1931 / 1680       | 1931 / 1680       | 1931 / 1680       |
