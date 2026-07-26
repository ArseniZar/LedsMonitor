#include "App.h"

App::App() : logger(Logger::init(LOGGER_DEBUG_MODE)),
             config(ConfigManager<DeviceLedConfigPair, NetworkConfigPair, TelegramBotConfigPair, WebServerConfigPair, AppConfigPair>::init(logger)),
             network(NetworkManager::init(logger)),
             server(WebServer::init(logger, config.getConfig<WebServerConfig>().port)),
             mac(MacAddress::init(network.getMacAddress())),
             bot(TelegramBot::init(logger, mac)),
             device(logger, mac, config.getConfig<DeviceLedConfig>().pin, config.getConfig<DeviceLedConfig>().countLed),
             
             webServerInactivityPeriodMs(WEBSERVER_INACTIVITY_PERIOD_MS),
             wifiReconnectTimer(WIFI_RECONNECT_PERIOD_MS, false, GTMode::Interval),
             applyConfigTimer(APPLY_CONFIG_TIMEOUT_MS, false, GTMode::Timeout),
             saveConfigTimer(SAVE_CONFIG_TIMEOUT_MS, false, GTMode::Timeout)

{
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{
    config.begin();

    device.applyConfig(config.getConfig<DeviceLedConfig>());
    device.begin();

    server.applyConfig(config.getConfig<WebServerConfig>());
    registerEndpoints();
    server.begin();

    network.applyConfig(config.getConfig<NetworkConfig>());
    network.begin();
    network.startMDNS();

    bot.applyConfig(config.getConfig<TelegramBotConfig>());
    registerCommands();
    bot.begin();

    this->applyConfig(config.getConfig<AppConfig>());
}

void App::start()
{
    network.startMDNS();
    server.start();
}

void App::update()
{
    if (network.getStatusWifi() == ConnState::WL_CONNECTED)
    {
        if (server.isCaptivePortalRunning())
        {
            network.stopCaptivePortal();
            server.stopCaptivePortal();

            auto networkConfig = config.getConfig<NetworkConfig>();
            if (strcmp(network.getSsid(), networkConfig.ssid) != 0 || strcmp(network.getPass(), networkConfig.password) != 0)
            {
                networkConfig.ssid = network.getSsid();
                networkConfig.password = network.getPass();
                config.updateConfig<NetworkConfig>(networkConfig);
                config.saveConfig<NetworkConfig>();
            }

            wifiReconnectTimer.stop();
        }

        if (millis() - server.getLastSystemRequestTime() > webServerInactivityPeriodMs)
        {
            network.stopAP();
        }

        bot.tick();
    }
    else
    {
        if (!server.isCaptivePortalRunning())
        {
            network.startAP();
            network.startCaptivePortal();
            server.startCaptivePortal(network.getAPIpAddress().toString().c_str());
            wifiReconnectTimer.start();
        }

        if ((millis() - server.getLastSystemRequestTime()) > webServerInactivityPeriodMs && wifiReconnectTimer.tick())
        {
            const auto &networkConfig = config.getConfig<NetworkConfig>();
            network.attemptConnectionAsync(networkConfig.ssid, networkConfig.password);
        }
    }

    if (applyConfigTimer.tick())
    {
        applyConfigTimer.stop();
        device.applyConfig(config.getConfig<DeviceLedConfig>());
        server.applyConfig(config.getConfig<WebServerConfig>());
        bot.applyConfig(config.getConfig<TelegramBotConfig>());
        network.applyConfig(config.getConfig<NetworkConfig>());
        this->applyConfig(config.getConfig<AppConfig>());
    }

    if (saveConfigTimer.tick())
    {
        saveConfigTimer.stop();
        config.save();
    }

    network.tick();
    server.tick();
}

void App::applyConfig(const AppConfig &config)
{
    if (webServerInactivityPeriodMs != config.webServerInactivityPeriodMs)
    {
        webServerInactivityPeriodMs = config.webServerInactivityPeriodMs;
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) webServerInactivityPeriodMs changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) webServerInactivityPeriodMs no changed"); return buf; });
    }

    if (wifiReconnectTimer.getTime() != config.wifiReconnectPeriodMs)
    {
        wifiReconnectTimer.setTime(config.wifiReconnectPeriodMs);

        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) wifiReconnectPeriodMs changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) wifiReconnectPeriodMs no changed"); return buf; });
    }

    if (applyConfigTimer.getTime() != config.applyConfigTimeoutMs)
    {
        applyConfigTimer.setTime(config.applyConfigTimeoutMs);

        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) applyConfigTimeoutMs changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) applyConfigTimeoutMs no changed"); return buf; });
    }

    if (saveConfigTimer.getTime() != config.saveConfigTimeoutMs)
    {
        saveConfigTimer.setTime(config.saveConfigTimeoutMs);

        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) saveConfigTimeoutMs changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) saveConfigTimeoutMs no changed"); return buf; });
    }
}

void App::registerCommands()
{
    using namespace api::telegram;

    bot.registerCommand<void, api::SuccessResponse<GetInfoLedDeviceResponse>>(
        "scan",
        [this]() -> api::SuccessResponse<GetInfoLedDeviceResponse>
        {
            return api::SuccessResponse<GetInfoLedDeviceResponse>(
                200,
                GetInfoLedDeviceResponse(
                    device.getName(),
                    network.getMdnsName(),
                    ModelBaseResponse("scan", device.getMacAddress().getMac())));
        });

    bot.registerCommand<UpdateLedDeviceRequest, void>(
        "update",
        [this](UpdateLedDeviceRequest &request) -> void
        {
            device.setColor(request.color);
            device.setPower(request.status);
        });

    bot.registerCommand<void, api::SuccessResponse<GetStateLedDeviceResponse>>(
        "state",
        [this]() -> api::SuccessResponse<GetStateLedDeviceResponse>
        {
            return api::SuccessResponse<GetStateLedDeviceResponse>(
                200,
                GetStateLedDeviceResponse(
                    device.getColor(),
                    device.getStatus(),
                    ModelBaseResponse("state", device.getMacAddress().getMac())));
        });
}

void App::registerEndpoints()
{
    using namespace api::webserver;

    server.registerEndpoint<void, api::SuccessResponse<ScanWifiNetworkStartedResponse>>(
        "/network/scan",
        HTTPMethod::POST,
        EndpointType::System,
        [this]() -> api::SuccessResponse<ScanWifiNetworkStartedResponse>
        {
            return api::SuccessResponse<ScanWifiNetworkStartedResponse>(
                200,
                ScanWifiNetworkStartedResponse(network.scanWifiNetworksAsync(), ModelBaseResponse()));
        });

    using ScanVariant = std::variant<
        api::SuccessResponse<ScanWifiNetworkResponse>,
        api::SuccessResponse<GetScanStatusResponse>>;

    server.registerEndpoint<void, ScanVariant>(
        "/network/scan",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> ScanVariant
        {
            ScanState scanStatus = network.getStatusScan();
            if (scanStatus == ScanState::COMPLETED)
            {
                return api::SuccessResponse<ScanWifiNetworkResponse>(
                    200,
                    ScanWifiNetworkResponse(
                        network.getScanWifiNetworksAsyncResults(),
                        ModelBaseResponse()));
            }
            else
            {
                return api::SuccessResponse<GetScanStatusResponse>(
                    200,
                    GetScanStatusResponse(network.getStatusScan(), ModelBaseResponse()));
            }
        });

    server.registerEndpoint<ConnectWifiNetworkRequest, api::SuccessResponse<ConnectWifiNetworkStartedResponse>>(
        "/network/connect",
        HTTPMethod::POST,
        EndpointType::System,
        [this](ConnectWifiNetworkRequest &request) -> api::SuccessResponse<ConnectWifiNetworkStartedResponse>
        {
            return api::SuccessResponse<ConnectWifiNetworkStartedResponse>(
                200,
                ConnectWifiNetworkStartedResponse(
                    network.attemptConnectionAsync(request.ssid.c_str(), request.password.c_str()),
                    ModelBaseResponse()));
        });

    server.registerEndpoint<void, api::SuccessResponse<GetWifiStatusResponse>>(
        "/network/connect",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetWifiStatusResponse>
        {
            return api::SuccessResponse<GetWifiStatusResponse>(
                200,
                GetWifiStatusResponse(network.getStatusWifi(), ModelBaseResponse()));
        });

    server.registerEndpoint<UpdateConfigRequest, void>(
        "/config",
        HTTPMethod::PATCH,
        EndpointType::System,
        [this](UpdateConfigRequest &request) -> void
        {
            DeviceLedConfig deviceLedConfig = config.getConfig<DeviceLedConfig>();
            fromOptional(request.countLed, deviceLedConfig.countLed);
            fromOptional(request.deviceName, deviceLedConfig.deviceName);

            TelegramBotConfig telegramBotConfig = config.getConfig<TelegramBotConfig>();
            fromOptional(request.token, telegramBotConfig.token);
            fromOptional(request.limitMessage, telegramBotConfig.limitMessage);
            fromOptional(request.periodUpdate, telegramBotConfig.periodUpdate);

            WebServerConfig webServerConfig = config.getConfig<WebServerConfig>();

            NetworkConfig networkConfig = config.getConfig<NetworkConfig>();
            fromOptional(request.apSsid, networkConfig.apSsid);
            fromOptional(request.apPassword, networkConfig.apPassword);
            fromOptional(request.mdnsName, networkConfig.mdnsName);
            fromOptional(request.wifiConnectionTimeoutMs, networkConfig.wifiConnectionTimeoutMs);

            AppConfig appConfig = config.getConfig<AppConfig>();

            config.updateConfig(deviceLedConfig);
            config.updateConfig(webServerConfig);
            config.updateConfig(telegramBotConfig);
            config.updateConfig(networkConfig);
            config.updateConfig(appConfig);

            applyConfigTimer.start();
            saveConfigTimer.start();
        });

    server.registerEndpoint<void, api::SuccessResponse<GetConfigResponse>>(
        "/config",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetConfigResponse>
        {
            const auto &networkConfig = config.getConfig<NetworkConfig>();
            const auto &deviceLedConfig = config.getConfig<DeviceLedConfig>();
            const auto &telegramBotConfig = config.getConfig<TelegramBotConfig>();

            return api::SuccessResponse<GetConfigResponse>(
                200,
                GetConfigResponse(
                    networkConfig.apSsid,
                    networkConfig.apPassword,
                    networkConfig.mdnsName,
                    networkConfig.wifiConnectionTimeoutMs,
                    deviceLedConfig.countLed,
                    deviceLedConfig.deviceName,
                    telegramBotConfig.token,
                    telegramBotConfig.limitMessage,
                    telegramBotConfig.periodUpdate,
                    ModelBaseResponse()));
        });

    // server.registerEndpoint<void, void>(
    //     "/stop",
    //     HTTPMethod::GET,
    // EndpointType::System,
    //     [this]() {
    //         network.stopAP();
    //         network.stopCaptivePortal();
    //         server.stopCaptivePortal();
    //         wifiReconnectTimer.stop();
    //     }
    // );
}