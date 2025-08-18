#include "App.h"

App::App() : cachedSsid(""), cachedPass(""),
             logger(Logger::init(LOGGER_DEBUG_MODE)),
             eeprom(Eeprom::init(SIZE_EEPROM)),
             wifi(WiFiSetup::init(logger)),
             mac(MacAddress::init(wifi.getMAC())),
             bot(TelegramBot::init(logger, BOT_TOKEN, mac)),
             device(DeviceLed<NeoBrgFeature, NeoEsp8266BitBangWs2811Method>(logger, mac, LED_COUNT, DEVICE_PIN))
{
    cachedSsid = eeprom.readString(ADDR_WIFI_SSID);
    cachedPass = eeprom.readString(ADDR_WIFI_PASS);
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{
#if ENABLE_WIFI_MODULE
    wifi.setMdnsName(MDNS_NAME);
    wifi.setAPConfig(AP_SSID, AP_PASS);
    wifi.setWiFiConfig(cachedSsid, cachedPass);
    wifi.begin();
#endif

#if ENABLE_WIFI_MODULE && ENABLE_TELEGRAM_BOT
    if (wifi.status() == ConnState::WL_CONNECTED)
    {
        commitWiFiIfChanged();
        #if ENABLE_TELEGRAM_BOT
                bot.setLimitMessage(BOT_LIMIT);
                bot.setPeriodUpdate(BOT_PERIOD);
                bot.begin();
        #endif
    }
#endif

#if ENABLE_DEVICE_LED
    device.begin();
#endif
}

void App::update()
{
#if ENABLE_WIFI_MODULE
    if (wifi.status() != ConnState::WL_CONNECTED)
    {
        wifi.setWiFiConfig(cachedSsid, cachedPass);
        wifi.begin();
        #if ENABLE_WIFI_MODULE
                if (wifi.status() == ConnState::WL_CONNECTED)
                {
                    commitWiFiIfChanged();
                }
        #endif
    }
    #if ENABLE_TELEGRAM_BOT
        else
        {
            bot.tick();
        }
    #endif
#endif
}

void App::commitWiFiIfChanged()
{
    String currentSsid = wifi.getSsid();
    String currentPass = wifi.getPass();

    if (currentSsid != cachedSsid || currentPass != cachedPass)
    {
        eeprom.writeString(currentSsid, ADDR_WIFI_SSID);
        eeprom.writeString(currentPass, ADDR_WIFI_PASS);

        cachedSsid = currentSsid;
        cachedPass = currentPass;
    }
}
