#include "main.h"

Logger &logger = Logger::init(LOGGER_DEBUG_MODE);
Eeprom &eeprom = Eeprom::init(512);
WiFiSetup &wifi = WiFiSetup::init(logger);
MacAddress &mac = MacAddress::init("48:E7:29:64:FF:9B", "FF:FF:FF:FF:FF:FF");
TelegramBot &bot = TelegramBot::init(logger, BOT_TOKEN, mac);

// LedController device(logger);

const String savedSsid = eeprom.readString(ADDR_SSID);
const String savedPass = eeprom.readString(ADDR_PASS);

void setup()
{
  Serial.begin(115200);
  if (LOGGER_DEBUG_MODE)
  {
    delay(3000);
  }

  bool status = wifi.begin(savedSsid, savedPass);

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
  bot.begin();
  bot.registerCommand<telegram::ModelBaseRequest, void>("scan", [](telegram::ModelBaseRequest &request)
                                                        { Serial.println(request.id); });
}

void loop()
{
  bot.tick();
}

// void setup()
// {
//   Serial.begin(115200);
//   delay(2000);
//   const su::Text text = "/command=update;mac=48:E7:29:64:FF:09;color=#e5757c;status=on/";
//   auto parse_ptr = telegram::parseTelegramRequest<telegram::UpdateLedDevice>(text);
//   if (parse_ptr->isOk())
//   {
//     auto *success_parse_ptr = static_cast<telegram::TelegramSuccessRequest<telegram::BaseModelRequest> *>(parse_ptr.get());
//     telegram::BaseModelRequest obj = success_parse_ptr->data;
//     Serial.println(obj.command);
//     Serial.println(obj.mac);
//     // Serial.println(obj.color);
//     // Serial.println(obj.status);
//   }
//   else
//   {
//     auto *error_ptr = static_cast<telegram::TelegramErrorRequest *>(parse_ptr.get());
//     Serial.println(error_ptr->message);
//   }
// }
// void loop()
// {
// }

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
