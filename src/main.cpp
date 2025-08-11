#include "main.h"

Logger &logger = Logger::init(LOGGER_DEBUG_MODE);
Eeprom &eeprom = Eeprom::init(512);
WiFiSetup &wifi = WiFiSetup::init(logger);

// LedController device(logger);
// TelegramBot bot(&logger, &eeprom);

const String savedSsid = eeprom.readString(ADDR_SSID);
const String savedPass = eeprom.readString(ADDR_PASS);

void setup()
{
  Serial.begin(115200);
  if (LOGGER_DEBUG_MODE)
  {
    delay(3000);
  }

  if (LOGGER_DEBUG_MODE)
  {
    bool status = wifi.begin("", "");
  }
  else{
    bool status = wifi.begin(savedSsid, savedPass);
  }
  if (status)
  {
    const String &newSsid = wifi.getSsid();
    const String &newPass = wifi.getPass();
    if (newSsid != savedSsid)
    {
      eeprom.writeString(newSsid, ADDR_SSID);
    }
    if (newPass != savedPass)
    {
      eeprom.writeString(newPass, ADDR_PASS);
    }
  }
}

void loop()
{
}

// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>

// // Конфигурация
// #define DEVICE_PIN    D5     // Пин данных (GPIO 4 на ESP8266)
// #define LED_COUNT  31        // Количество светодиодов в ленте
// #define BRIGHTNESS 255        // Яркость (0–255)

// // Инициализация объекта NeoPixel для WS2811
// Adafruit_NeoPixel strip(LED_COUNT, DEVICE_PIN, NEO_GRB + NEO_KHZ400);

// void setup() {
//   // Инициализация ленты
//   strip.begin();
//   strip.setBrightness(BRIGHTNESS); // Установка яркости
//   strip.show(); // Очистка ленты при старте

//   // Установка чистого белого цвета без гамма-коррекции
//   for(int i = 0; i < strip.numPixels(); i++) {
//     uint32_t white = strip.Color(255, 255, 255);
//     strip.setPixelColor(i, strip.gamma32(white)); // Гамма-коррекция
//   }
//   strip.show();
// }

// void loop() {
//   // Ничего не делаем, цвет установлен в setup()
// }
