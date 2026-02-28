#include "ConfigDefault.h"

// Базовая сетевая конфигурация:
// ssid / password     - сохранённая Wi‑Fi сеть по умолчанию отсутствует (пустые строки)
// "SmartHome"        - SSID точки доступа, которую поднимает устройство в режиме портала
// "12345678"         - пароль точки доступа (минимум 8 символов)
// "smarthome"        - mDNS‑имя: устройство будет доступно как smarthome.local
const NetworkConfig& DEFAULT_NETWORK_CONFIG = NetworkConfig::defaultConfig(
    "",          // ssid (not saved by default)
    "",          // password
    "SmartHome", // AP SSID
    "12345678",  // AP password
    "smarthome"  // mDNS name
);


// Базовая конфигурация LED‑устройства на прошивке "из коробки".
// 3  - аппаратный пин данных (см. DEVICE_PIN в config.h)
// 60 - количество светодиодов в ленте по умолчанию
// "Led" - имя устройства, отображаемое в UI / Telegram
const DeviceLedConfig& DEFAULT_DEVICE_CONFIG = DeviceLedConfig::defaultConfig(
    3,
    60,
    "Led"
);


// Базовая конфигурация Telegram‑бота:
// token         - по умолчанию пустой, пока пользователь не введёт свой токен
// limitMessage  - максимальное количество сообщений за один тик (анти‑спам)
// periodUpdate  - период опроса Telegram (мс)
const TelegramBotConfig& DEFAULT_TELEGRAM_CONFIG = TelegramBotConfig::defaultConfig(
    "",   // token
    10,    // limitMessage
    1000   // periodUpdate
);
