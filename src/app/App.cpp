#include "App.h"

App::App() : savedWifiData(),
             storage(Storage<SavedWifiData>("/data.dat", 'W')),
             logger(Logger::init(LOGGER_DEBUG_MODE)),
             network(NetworkManager::init(logger, AP_SSID, AP_PASS, MDNS_NAME)),
             mac(MacAddress::init(network.getMacAddress())),
             bot(TelegramBot::init(logger, BOT_TOKEN, mac)),
             device(DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod>(logger, mac, DEVICE_NAME, LED_COUNT, DEVICE_PIN))
{
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{

#if ENABLE_STORAGE_MODULE
    storage.begin();
    savedWifiData = storage.readData();
#endif

#if ENABLE_DEVICE_MODULE
    device.begin();
#endif

#if ENABLE_WIFI_MODULE
    network.setMdnsName(MDNS_NAME);
    network.setAPConfig(AP_SSID, AP_PASS);
    network.setWiFiConfig(savedWifiData.ssid, savedWifiData.password);
    network.begin();
#endif

#if ENABLE_WIFI_MODULE
    if (network.statusWifi() == ConnState::WL_CONNECTED)
    {
        commitWiFiIfChanged();
    }
#endif

#if ENABLE_TELEGRAM_BOT_MODULE
    bot.setLimitMessage(BOT_LIMIT);
    bot.setPeriodUpdate(BOT_PERIOD);
    bot.begin();

#if ENABLE_DEVICE_MODULE 
    bindDeviceToTelegramCommands();
#endif
#endif
}

void App::update()
{
#if ENABLE_WIFI_MODULE
    if (network.statusWifi() != ConnState::WL_CONNECTED)
    {
        network.setWiFiConfig(savedWifiData.ssid, savedWifiData.password);
        network.begin();
        if (network.statusWifi() == ConnState::WL_CONNECTED)
        {
            commitWiFiIfChanged();
        }
    }
    else
    {
#endif

#if ENABLE_TELEGRAM_BOT_MODULE
        bot.tick();
#endif

#if ENABLE_WIFI_MODULE
    }
#endif
}

void App::commitWiFiIfChanged()
{
    String32 currentSsid = network.getSsid();
    String32 currentPass = network.getPass();

    if (currentSsid != savedWifiData.ssid || currentPass != savedWifiData.password)
    {
        savedWifiData.ssid = currentSsid;
        savedWifiData.password = currentPass;
#if ENABLE_STORAGE_MODULE
        storage.writeData(savedWifiData);
#endif
    }
}

void App::bindDeviceToTelegramCommands()
{
    using namespace telegram;

    bot.registerCommand<ScanLedDeviceRequest, ScanLedDeviceResponse>(CMD_SCAN, [this](ScanLedDeviceRequest &request) -> ScanLedDeviceResponse
        { 
            return ScanLedDeviceResponse(device.getName(), std::move(ModelBaseResponse(request.command, device.getMacAddress().getMac()))); 
        }
    );
    bot.registerCommand<UpdateLedDeviceRequest, void>(CMD_UPDATE, [this](UpdateLedDeviceRequest &request) -> void
        {
            device.setColor(request.color); device.setPower(request.status); 
        }
    );
    bot.registerCommand<GetLedDeviceRequest, GetLedDeviceResponse>(CMD_GET,[this](GetLedDeviceRequest& request) -> GetLedDeviceResponse
        {
            return GetLedDeviceResponse(device.getColor(),device.getStatus(),std::move(ModelBaseResponse(request.command,device.getMacAddress().getMac())));
        }
    );
}