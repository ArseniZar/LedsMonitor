#include "App.h"

App::App() : logger(Logger::init(LOGGER_DEBUG_MODE)),
             config(ConfigManager<DeviceLedConfigPair, NetworkConfigPair, TelegramBotConfigPair, WebServerConfigPair>::init(logger)),
             network(NetworkManager::init(logger)),
             server(WebServer::init(logger, config.getConfig<WebServerConfig>().port)),
             mac(MacAddress::init(network.getMacAddress())),
             bot(TelegramBot::init(logger, mac)),
             device(logger, mac, config.getConfig<DeviceLedConfig>().pin, config.getConfig<DeviceLedConfig>().countLed)
{
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{

#if ENABLE_CONFIG_MODULE
    config.begin();
#endif

#if ENABLE_DEVICE_MODULE
#if ENABLE_CONFIG_MODULE
    device.applyConfig(config.getConfig<DeviceLedConfig>());
#endif

    device.begin();

#endif

#if ENABLE_WEBSERVER_MODULE
#if ENABLE_CONFIG_MODULE
    server.applyConfig(config.getConfig<WebServerConfig>());
#endif

#if ENABLE_NETWORK_MODULE && ENABLE_CONFIG_MODULE
    registerRoutes();
#endif

    server.begin();

#endif

#if ENABLE_NETWORK_MODULE
#if ENABLE_CONFIG_MODULE
    network.applyConfig(config.getConfig<NetworkConfig>());
#endif

    network.begin();

#endif

#if ENABLE_TELEGRAM_BOT_MODULE
#if ENABLE_CONFIG_MODULE
    bot.applyConfig(config.getConfig<TelegramBotConfig>());
#endif

#if ENABLE_DEVICE_MODULE
    registerCommands();
#endif

    bot.begin();

#endif
}

void App::update()
{
    if (!server.isRunning())
    {
        network.startWebServerNetwork();
        server.start();
    }

    if (server.isRunning())
    {
        server.tick();
    }

// #if ENABLE_NETWORK_MODULE
//     if (network.getStatusWifi() != ConnState::WL_CONNECTED)
//     {
//         if(!server.isRunning()){
//             network.startWebServerNetwork();
//             server.start();
//         }
//         // // network.setWifiConfig(savedWifiData.ssid, savedWifiData.password);
//         // // network.begin();
//         // if (network.getStatusWifi() == ConnState::WL_CONNECTED)
//         // {
//         //     // commitWiFiIfChanged();
//         // }
//     }
//     else
//     {
// #endif

// #if ENABLE_TELEGRAM_BOT_MODULE
//         bot.tick();
// #endif

// #if ENABLE_NETWORK_MODULE
//     }
// #endif
}

void App::registerCommands()
{
    using namespace api::telegram;

    bot.registerCommand<void, api::SuccessResponse<GetInfoLedDeviceResponse>>("scan", [this]() -> api::SuccessResponse<GetInfoLedDeviceResponse>
                                                                              { return api::SuccessResponse<GetInfoLedDeviceResponse>(200, GetInfoLedDeviceResponse(device.getName(), std::move(ModelBaseResponse("scan", device.getMacAddress().getMac())))); });

    bot.registerCommand<UpdateLedDeviceRequest, void>("update", [this](UpdateLedDeviceRequest &request) -> void
                                                      { device.setColor(request.color); device.setPower(request.status); });

    bot.registerCommand<void, api::SuccessResponse<GetStateLedDeviceResponse>>("state", [this]() -> api::SuccessResponse<GetStateLedDeviceResponse>
                                                                               { return api::SuccessResponse<GetStateLedDeviceResponse>(200, GetStateLedDeviceResponse(device.getColor(), device.getStatus(), std::move(ModelBaseResponse("state", device.getMacAddress().getMac())))); });
}

void App::registerRoutes()
{
    using namespace api::webserver;

    server.registerRoute<void, api::SuccessResponse<ScanWifiNetworkStartedResponse>>("/network/scan", HTTPMethod::POST, [this]() -> api::SuccessResponse<ScanWifiNetworkStartedResponse>
                                                                                     { return api::SuccessResponse<ScanWifiNetworkStartedResponse>(200, ScanWifiNetworkStartedResponse(network.scanWifiNetworksAsync(), std::move(ModelBaseResponse()))); });

    server.registerRoute<void, std::variant<api::SuccessResponse<ScanWifiNetworkResponse>, api::SuccessResponse<GetScanStatusResponse>>>("/network/scan", HTTPMethod::GET, [this]() -> std::variant<api::SuccessResponse<ScanWifiNetworkResponse>, api::SuccessResponse<GetScanStatusResponse>>
                                                                                                                                         {   
        ScanState scanStatus = network.getStatusScan(); 
        if(scanStatus == ScanState::COMPLETED)
        {
            return api::SuccessResponse<ScanWifiNetworkResponse>(200, ScanWifiNetworkResponse(std::move(network.getScanWifiNetworksAsyncResults()), std::move(ModelBaseResponse())));
        }
        else
        {
            return api::SuccessResponse<GetScanStatusResponse>(200, GetScanStatusResponse(network.getStatusScan(), std::move(ModelBaseResponse())));
        } });

    server.registerRoute<ConnectWifiNetworkRequest, api::SuccessResponse<ConnectWifiNetworkStartedResponse>>("/network/connect", HTTPMethod::POST, [this](ConnectWifiNetworkRequest &request) -> api::SuccessResponse<ConnectWifiNetworkStartedResponse>
                                                                                                             { return api::SuccessResponse<ConnectWifiNetworkStartedResponse>(200, ConnectWifiNetworkStartedResponse(network.attemptConnectionAsync(request.ssid.c_str(), request.password.c_str()), std::move(ModelBaseResponse()))); });

    server.registerRoute<void, api::SuccessResponse<GetWifiStatusResponse>>("/network/connect", HTTPMethod::GET, [this]() -> api::SuccessResponse<GetWifiStatusResponse>
                                                                            { return api::SuccessResponse<GetWifiStatusResponse>(200, GetWifiStatusResponse(network.getStatusWifi(), std::move(ModelBaseResponse()))); });

    server.registerRoute<UpdateConfigRequest, void>("/config", HTTPMethod::PATCH, [this](UpdateConfigRequest &request) -> void
                                                    {
                                                         NetworkConfig networkConfig = config.getConfig<NetworkConfig>();
                                                         fromOptional(request.apSsid, networkConfig.apSsid);
                                                         fromOptional(request.apPassword, networkConfig.apPassword);
                                                         fromOptional(request.mdnsName, networkConfig.mdnsName);
                                                         fromOptional(request.wifiConnectionTimeout, networkConfig.wifiConnectionTimeout);

                                                         DeviceLedConfig deviceLedConfig = config.getConfig<DeviceLedConfig>();
                                                         fromOptional(request.countLed, deviceLedConfig.countLed);
                                                         fromOptional(request.deviceName, deviceLedConfig.deviceName);

                                                         TelegramBotConfig telegramBotConfig = config.getConfig<TelegramBotConfig>();
                                                         fromOptional(request.token, telegramBotConfig.token);
                                                         fromOptional(request.limitMessage, telegramBotConfig.limitMessage);
                                                         fromOptional(request.periodUpdate, telegramBotConfig.periodUpdate);

                                                         config.updateConfig(networkConfig);
                                                         config.updateConfig(deviceLedConfig);
                                                         config.updateConfig(telegramBotConfig); });

    server.registerRoute<void, api::SuccessResponse<GetConfigResponse>>("/config", HTTPMethod::GET, [this]() -> api::SuccessResponse<GetConfigResponse>
                                                                        {
        const auto &networkConfig = config.getConfig<NetworkConfig>();
        const auto &deviceLedConfig = config.getConfig<DeviceLedConfig>();
        const auto &telegramBotConfig = config.getConfig<TelegramBotConfig>();

        return api::SuccessResponse<GetConfigResponse>(200, GetConfigResponse(networkConfig.apSsid, networkConfig.apPassword, networkConfig.mdnsName, networkConfig.wifiConnectionTimeout,
                                     deviceLedConfig.countLed, deviceLedConfig.deviceName,
                                     telegramBotConfig.token, telegramBotConfig.limitMessage, telegramBotConfig.periodUpdate,
                                     ModelBaseResponse())); });

    server.registerRoute<void, void>("/stop", HTTPMethod::GET, [this]()
                                     { server.stop(); });
}