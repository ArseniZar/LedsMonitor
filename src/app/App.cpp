#include "App.h"

App::App() : mode(AppMode::WORK),
             logger(Logger::init(LOGGER_DEBUG_MODE)),
             config(ConfigManager<DeviceLedConfigPair, NetworkConfigPair, TelegramBotConfigPair, WebServerConfigPair, AppConfigPair>::init(logger)),
             network(NetworkManager::init(logger)),
             server(WebServer::init(logger, config.getConfig<WebServerConfig>().port)),
             mac(MacAddress::init(network.getMacAddress())),
             bot(TelegramBot::init(logger, mac)),
             device(logger, mac, config.getConfig<DeviceLedConfig>().pin, config.getConfig<DeviceLedConfig>().countLed),

             webServerInactivityPeriodMs(WEBSERVER_INACTIVITY_PERIOD_MS),
             wifiReconnectTimer(WIFI_RECONNECT_PERIOD_MS, false, GTMode::Interval),
             applyConfigTimer(APPLY_CONFIG_TIMEOUT_MS, false, GTMode::Overflow),
             saveConfigTimer(SAVE_CONFIG_TIMEOUT_MS, false, GTMode::Timeout),
             buttonHoldTimer(HOLD_BUTTON_TIMEOUT_MS, false, GTMode::Timeout),

             button(config.getConfig<AppConfig>().buttonPin, INPUT_PULLUP)

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
    config.reset();

    device.applyConfig(config.getConfig<DeviceLedConfig>());
    device.begin();

    server.applyConfig(config.getConfig<WebServerConfig>());
    registerEndpoints();
    server.begin();

    network.applyConfig(config.getConfig<NetworkConfig>());
    network.begin();

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

void App::startMode(AppMode newMode)
{
    if (mode == newMode)
        return;

    switch (newMode)
    {
    case AppMode::WORK:
    {
        mode = AppMode::WORK;
        server.stopCaptivePortal();
        network.stopDNS();
        network.stopAP();
        wifiReconnectTimer.stop();
        return;
    }
    case AppMode::CONFIG:
    {
        mode = AppMode::CONFIG;
        network.startAP();
        network.startDNS();
        server.startCaptivePortal(network.getAPIpAddress().toString().c_str());
        wifiReconnectTimer.start();
        return;
    }
    }
}

void App::update()
{
    network.tick();
    server.tick();
    button.tick();

    const bool isWifiConnected = (network.getStatusWifi() == ConnState::WL_CONNECTED);
    const bool isServerInactive = ((millis() - server.getLastSystemRequestTime()) > webServerInactivityPeriodMs);

    if (mode == AppMode::WORK)
    {
        if (button.hold())
        {
            buttonHoldTimer.start();
            startMode(AppMode::CONFIG);
        }
        else if (!isWifiConnected)
        {
            startMode(AppMode::CONFIG);
        }
    }
    else if (mode == AppMode::CONFIG)
    {
        const bool buttonHoldTimerCheck = (!buttonHoldTimer.running() || buttonHoldTimer.tick());
        const bool applyConfigTimerCheck = (!applyConfigTimer.running() || applyConfigTimer.tick());

        if (isWifiConnected && isServerInactive && buttonHoldTimerCheck && applyConfigTimerCheck)
        {
            startMode(AppMode::WORK);
        }
    }

    if (this->mode == AppMode::WORK)
    {
        bot.tick();
    }
    else if (mode == AppMode::CONFIG)
    {
        if (!isWifiConnected && isServerInactive && wifiReconnectTimer.tick())
        {
            const auto &networkConfig = config.getConfig<NetworkConfig>();
            network.attemptConnectionAsync(networkConfig.ssid, networkConfig.password);
        }

        if (isWifiConnected)
        {
            auto networkConfig = config.getConfig<NetworkConfig>();
            const bool ssidChanged = (strcmp(network.getSsid(), networkConfig.ssid.c_str()) != 0);
            const bool passChanged = (strcmp(network.getPass(), networkConfig.password.c_str()) != 0);

            if (ssidChanged || passChanged)
            {
                networkConfig.ssid = network.getSsid();
                networkConfig.password = network.getPass();
                config.updateConfig(networkConfig);
            }
        }
    }

    if (applyConfigTimer.tick())
    {
        device.applyConfig(config.getConfig<DeviceLedConfig>());
        server.applyConfig(config.getConfig<WebServerConfig>());
        bot.applyConfig(config.getConfig<TelegramBotConfig>());
        network.applyConfig(config.getConfig<NetworkConfig>());
        this->applyConfig(config.getConfig<AppConfig>());
        applyConfigTimer.stop();
    }

    if ((!applyConfigTimer.running() || applyConfigTimer.tick()) && saveConfigTimer.tick())
    {
        config.save();
    }
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

    if (buttonHoldTimer.getTime() != config.holdButtonTimeoutMs)
    {
        buttonHoldTimer.setTime(config.holdButtonTimeoutMs);

        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) holdButtonTimeoutMs changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(App::applyConfig) holdButtonTimeoutMs no changed"); return buf; });
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
        "/api/v1/network/scan",
        HTTPMethod::POST,
        EndpointType::System,
        [this]() -> api::SuccessResponse<ScanWifiNetworkStartedResponse>
        {
            return api::SuccessResponse<ScanWifiNetworkStartedResponse>(
                200,
                ScanWifiNetworkStartedResponse(network.scanWifiNetworksAsync(), ModelBaseResponse()));
        });

    server.registerEndpoint<void, std::variant<api::SuccessResponse<ScanWifiNetworkResponse>, api::SuccessResponse<GetScanStatusResponse>>>(
        "api/v1/network/scan",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> std::variant<api::SuccessResponse<ScanWifiNetworkResponse>, api::SuccessResponse<GetScanStatusResponse>>
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
        "/api/v1/network/connect",
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
        "/api/v1/network/connect",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetWifiStatusResponse>
        {
            return api::SuccessResponse<GetWifiStatusResponse>(
                200,
                GetWifiStatusResponse(network.getStatusWifi(), ModelBaseResponse()));
        });

    server.registerEndpoint<UpdateNetworkConfigRequest, void>(
        "/api/v1/config/network",
        HTTPMethod::PATCH,
        EndpointType::System,
        [this](UpdateNetworkConfigRequest &request) -> void
        {
            NetworkConfig networkConfig = config.getConfig<NetworkConfig>();
            fromOptional(request.apSsid, networkConfig.apSsid);
            fromOptional(request.apPassword, networkConfig.apPassword);
            fromOptional(request.mdnsName, networkConfig.mdnsName);
            fromOptional(request.wifiConnectionTimeoutMs, networkConfig.wifiConnectionTimeoutMs);

            config.updateConfig(networkConfig);
            applyConfigTimer.start();
        });

    server.registerEndpoint<void, api::SuccessResponse<GetNetworkConfigResponse>>(
        "/api/v1/config/network",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetNetworkConfigResponse>
        {
            const auto &networkConfig = config.getConfig<NetworkConfig>();
            return api::SuccessResponse<GetNetworkConfigResponse>(
                200,
                GetNetworkConfigResponse(
                    networkConfig.ssid,
                    networkConfig.password,
                    networkConfig.apSsid,
                    networkConfig.apPassword,
                    networkConfig.mdnsName,
                    networkConfig.wifiConnectionTimeoutMs,
                    ModelBaseResponse()));
        });

    server.registerEndpoint<void, void>(
        "/api/v1/config/network",
        HTTPMethod::DELETE,
        EndpointType::System,
        [this]()
        {
            config.resetConfig<NetworkConfig>();
            applyConfigTimer.start();
        });

    server.registerEndpoint<UpdateDeviceLedConfigRequest, void>(
        "/api/v1/config/deviceled",
        HTTPMethod::PATCH,
        EndpointType::System,
        [this](UpdateDeviceLedConfigRequest &request) -> void
        {
            DeviceLedConfig deviceLedConfig = config.getConfig<DeviceLedConfig>();
            fromOptional(request.countLed, deviceLedConfig.countLed);
            fromOptional(request.deviceName, deviceLedConfig.deviceName);

            config.updateConfig(deviceLedConfig);
            applyConfigTimer.start();
        });

    server.registerEndpoint<void, api::SuccessResponse<GetDeviceLedConfigResponse>>(
        "/api/v1/config/deviceled",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetDeviceLedConfigResponse>
        {
            const auto &deviceLedConfig = config.getConfig<DeviceLedConfig>();
            return api::SuccessResponse<GetDeviceLedConfigResponse>(
                200,
                GetDeviceLedConfigResponse(
                    deviceLedConfig.pin,
                    deviceLedConfig.countLed,
                    deviceLedConfig.deviceName,
                    ModelBaseResponse()));
        });

    server.registerEndpoint<void, void>(
        "/api/v1/config/deviceled",
        HTTPMethod::DELETE,
        EndpointType::System,
        [this]()
        {
            config.resetConfig<DeviceLedConfig>();
            applyConfigTimer.start();
        });

    server.registerEndpoint<UpdateTelegramBotConfigRequest, void>(
        "/api/v1/config/telegrambot",
        HTTPMethod::PATCH,
        EndpointType::System,
        [this](UpdateTelegramBotConfigRequest &request) -> void
        {
            TelegramBotConfig telegramBotConfig = config.getConfig<TelegramBotConfig>();
            fromOptional(request.token, telegramBotConfig.token);
            fromOptional(request.limitMessage, telegramBotConfig.limitMessage);
            fromOptional(request.periodUpdate, telegramBotConfig.periodUpdate);

            config.updateConfig(telegramBotConfig);
            applyConfigTimer.start();
        });

    server.registerEndpoint<void, api::SuccessResponse<GetTelegramBotConfigResponse>>(
        "/api/v1/config/telegrambot",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetTelegramBotConfigResponse>
        {
            const auto &telegramBotConfig = config.getConfig<TelegramBotConfig>();
            return api::SuccessResponse<GetTelegramBotConfigResponse>(
                200,
                GetTelegramBotConfigResponse(
                    telegramBotConfig.token,
                    telegramBotConfig.limitMessage,
                    telegramBotConfig.periodUpdate,
                    ModelBaseResponse()));
        });

    server.registerEndpoint<void, void>(
        "/api/v1/config/telegrambot",
        HTTPMethod::DELETE,
        EndpointType::System,
        [this]()
        {
            config.resetConfig<TelegramBotConfig>();
            applyConfigTimer.start();
        });

    server.registerEndpoint<UpdateAppConfigRequest, void>(
        "/api/v1/config/app",
        HTTPMethod::PATCH,
        EndpointType::System,
        [this](UpdateAppConfigRequest &request) -> void
        {
            AppConfig appConfig = config.getConfig<AppConfig>();
            fromOptional(request.webServerInactivityPeriodMs, appConfig.webServerInactivityPeriodMs);
            fromOptional(request.wifiReconnectPeriodMs, appConfig.wifiReconnectPeriodMs);
            fromOptional(request.applyConfigTimeoutMs, appConfig.applyConfigTimeoutMs);
            fromOptional(request.saveConfigTimeoutMs, appConfig.saveConfigTimeoutMs);
            fromOptional(request.holdButtonTimeoutMs, appConfig.holdButtonTimeoutMs);

            config.updateConfig(appConfig);
            applyConfigTimer.start();
        });

    server.registerEndpoint<void, api::SuccessResponse<GetAppConfigResponse>>(
        "/api/v1/config/app",
        HTTPMethod::GET,
        EndpointType::System,
        [this]() -> api::SuccessResponse<GetAppConfigResponse>
        {
            const auto &appConfig = config.getConfig<AppConfig>();
            return api::SuccessResponse<GetAppConfigResponse>(
                200,
                GetAppConfigResponse(
                    appConfig.webServerInactivityPeriodMs,
                    appConfig.wifiReconnectPeriodMs,
                    appConfig.applyConfigTimeoutMs,
                    appConfig.saveConfigTimeoutMs,
                    appConfig.holdButtonTimeoutMs,
                    appConfig.buttonPin,
                    ModelBaseResponse()));
        });

    server.registerEndpoint<void, void>(
        "/api/v1/config/app",
        HTTPMethod::DELETE,
        EndpointType::System,
        [this]()
        {
            config.resetConfig<AppConfig>();
            applyConfigTimer.start();
        });
    
    server.registerEndpoint<void, void>(
        "/api/v1/config",
        HTTPMethod::POST,
        EndpointType::System,
        [this]()
        {
            saveConfigTimer.start();
        });
}