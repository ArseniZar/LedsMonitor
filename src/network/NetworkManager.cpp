#include "NetworkManager.h"

NetworkManager &NetworkManager::init(Logger &logger)
{
    static NetworkManager instance(logger);
    return instance;
}

NetworkManager::NetworkManager(Logger &logger)
    : logger(logger),
      dnsServer(),
      apSsid(F(AP_SSID)),
      apPassword(F(AP_PASS)),
      ssid(F("")),
      password(F("")),
      attemptSsid(F(WIFI_ATTEMPT_SSID)),
      attemptPassword(F(WIFI_ATTEMPT_PASS)),
      mdnsName(F(MDNS_NAME)),
      wifiConnectionTimeout(WIFI_CONNECTION_TIMEOUT_MS),
      onGotIpHandlers{},
      onDisconnectedHandlers{}
{
}

ConnState NetworkManager::getStatusWifi() const
{
    return static_cast<ConnState>(WiFi.status());
}

ScanState NetworkManager::getStatusScan() const
{
    int result = WiFi.scanComplete();

    if (result >= 0)
    {
        return ScanState::COMPLETED;
    }

    return static_cast<ScanState>(result);
}

bool NetworkManager::begin()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::begin) Starting Network Manager..."); return buf; });

    if (attemptConnection())
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::begin) Starting Network Manager finished successfully."); return buf; });
        return true;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::begin) Failed to connect to Wi-Fi network."); return buf; });

    return false;
}

void NetworkManager::tick()
{
    if (MDNS.isRunning())
    {
        MDNS.update();
    }

    if (dnsServer.isForwarding())
    {
        dnsServer.processNextRequest();
    }
}

bool NetworkManager::startCaptivePortal()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::startCaptivePortal) Starting web server network (AP + DNS)..."); return buf; });

    if (!startAP())
    {
        logger.log(LOG_ERROR, [&]() -> String128
                   { String128 buf;  buf = F("(NetworkManager::startCaptivePortal) Failed to start Wi-Fi Access Point for web server."); return buf; });
        return false;
    }

    if (!startDNS())
    {
        logger.log(LOG_ERROR, [&]() -> String128
                   { String128 buf;  buf = F("(NetworkManager::startCaptivePortal) Failed to start DNS server for web server."); return buf; });
        return false;
    }

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::startCaptivePortal) Web server network started successfully (AP + DNS)."); return buf; });

    return true;
}

bool NetworkManager::stopCaptivePortal()
{
    bool dnsStopped = stopDNS();
    bool apStopped = stopAP();

    if (apStopped && dnsStopped)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::stopCaptivePortal) Web server network stopped successfully (AP + DNS)."); return buf; });
        return true;
    }

    if (!apStopped)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::stopCaptivePortal) Failed to stop Wi-Fi Access Point for web server."); return buf; });
    }

    if (!dnsStopped)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::stopCaptivePortal) Failed to stop DNS server for web server."); return buf; });
    }
    return false;
}

void NetworkManager::configureWifiPerformance()
{
    bool sleepModeSet = WiFi.setSleepMode(WIFI_NONE_SLEEP, 50);
    logger.log(sleepModeSet ? LOG_DEBUG : LOG_WARN, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::configureWifiPerformance) Wi-Fi sleep mode WIFI_NONE_SLEEP with 50 ms delay "));
                buf.add(sleepModeSet ? F("enabled successfully") : F("failed to enable"));
                return buf; });

    WiFi.setOutputPower(20.5f);
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf.add(F("(NetworkManager::configureWifiPerformance) Wi-Fi output power set to 20.5 dBm")); return buf; });
}

bool NetworkManager::attemptConnection()
{
    return attemptConnection(attemptSsid.c_str(), attemptPassword.c_str());
}

bool NetworkManager::attemptConnection(const char *ssid, const char *password)
{
    for (int i = 0; i < MAX_WIFI_HANDLER; i++)
    {
        this->onGotIpHandlers[i] = nullptr;
        this->onDisconnectedHandlers[i] = nullptr;
    }

    this->attemptSsid = ssid;
    this->attemptPassword = password;

    if (!tryConnectWifi(attemptSsid, attemptPassword))
    {
        this->attemptSsid = F("");
        this->attemptPassword = F("");
        return false;
    }

    this->ssid = attemptSsid;
    this->password = attemptPassword;
    this->attemptSsid = F("");
    this->attemptPassword = F("");
    return true;
}

bool NetworkManager::attemptConnectionAsync()
{
    return attemptConnectionAsync(attemptSsid.c_str(), attemptPassword.c_str());
}

bool NetworkManager::attemptConnectionAsync(const char *ssid, const char *password)
{
    for (int i = 0; i < MAX_WIFI_HANDLER; i++)
    {
        this->onGotIpHandlers[i] = nullptr;
        this->onDisconnectedHandlers[i] = nullptr;
    }

    this->attemptSsid = ssid;
    this->attemptPassword = password;

    if (!tryConnectWifiAsync(attemptSsid, attemptPassword))
    {
        this->attemptSsid = F("");
        this->attemptPassword = F("");
        return false;
    }

    onGotIpHandlers[1] = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
                                                 {
        this->ssid = this->attemptSsid;
        this->password = this->attemptPassword;
        this->attemptSsid = F("");
        this->attemptPassword = F("");
        this->onGotIpHandlers[1] = nullptr; 
        this->onDisconnectedHandlers[1] = nullptr; });

    onDisconnectedHandlers[1] = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                                               {
                                                                   // switch(static_cast<WifiFailState>(event.reason))
                                                                   // {
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_NO_AP_FOUND:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_AUTH_FAIL:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_ASSOC_FAIL:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_BEACON_TIMEOUT:

                                                                   this->attemptSsid = F("");
                                                                   this->attemptPassword = F("");
                                                                   this->onGotIpHandlers[1] = nullptr;
                                                                   this->onDisconnectedHandlers[1] = nullptr;

                                                                   //     break;
                                                                   // }
                                                               });

    return true;
}

void NetworkManager::applyConfig(const NetworkConfig &config)
{
    if (wifiConnectionTimeout != config.wifiConnectionTimeout)
    {
        setWifiConnectionTimeout(config.wifiConnectionTimeout);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) WifiConnectionTimeout changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) WifiConnectionTimeout no changed"); return buf; });
    }

    if ((!(strcmp(ssid, config.ssid) == 0) && !(strcmp(attemptSsid, config.ssid) == 0)) || (!(strcmp(password, config.password) == 0) && !(strcmp(attemptPassword, config.password) == 0))) // FIXME: Временный костыль из-за отсутствия operator== в StringN.
    {
        setAttemptWifiConfig(config.ssid, config.password);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) Ssid Password changed"); return buf; });

        if (WiFi.getMode() == WIFI_STA || WiFi.getMode() == WIFI_AP_STA)
        {
            attemptConnectionAsync();
        }
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) Ssid Password changed"); return buf; });
    }

    if ((!(strcmp(apSsid, config.apSsid) == 0)) || (!(strcmp(apPassword, config.apPassword)) == 0))
    {
        setAPConfig(config.apSsid, config.apPassword);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) apSsid apPassword changed"); return buf; });

        if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA)
        {
            startAP();
        }
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) apSsid apPassword no changed"); return buf; });
    }

    if (!(strcmp(mdnsName, config.mdnsName) == 0))
    {
        setMdnsName(config.mdnsName);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) MdnsName changed"); return buf; });
        if (MDNS.isRunning())
        {
            startMDNS();
        }
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::applyConfig) MdnsName no changed"); return buf; });
    }
}

void NetworkManager::setAttemptWifiConfig(const char *ssid, const char *password)
{
    this->attemptSsid = ssid;
    this->attemptPassword = password;
}

void NetworkManager::setAPConfig(const char *apSsid, const char *apPassword)
{
    this->apSsid = apSsid;
    this->apPassword = apPassword;
}

bool NetworkManager::startAP()
{
    return startAP(apSsid, apPassword);
}

bool NetworkManager::startAP(const String32 &apSsid, const String32 &apPassword)
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startAP) Attempting to start Wi-Fi Access Point with SSID: '"));
                buf.add(apSsid);
                buf.add(F("'"));
                return buf; });

    if (apSsid.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::startAP) Failed to start Access Point: SSID is empty."));
                    return buf; });
        return false;
    }

    WiFi.enableAP(true);

    bool phySet = WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    logger.log(phySet ? LOG_DEBUG : LOG_WARN, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startAP) PHY mode set to WIFI_PHY_MODE_11G with 50 ms delay "));
                buf.add(phySet ? F("enabled successfully") : F("failed to enable"));
                return buf; });

    bool apStarted = WiFi.softAP(apSsid, apPassword);
    logger.log(apStarted ? LOG_INFO : LOG_ERROR, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startAP) Starting Wi-Fi Access Point "));
                buf.add(apStarted ? F("succeeded.") : F("failed. Please check configuration."));
                return buf; });

    if (!apStarted)
        return false;

    IPAddress apIP = WiFi.softAPIP();
    bool ipValid = (apIP != IPAddress(0, 0, 0, 0));
    logger.log(ipValid ? LOG_INFO : LOG_ERROR, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startAP) AP IP address "));
                buf.add(ipValid ? F("is valid: ") : F("is invalid after starting softAP!"));
                if (ipValid) buf.add(apIP.toString().c_str());
                return buf; });

    if (!ipValid)
        return false;

    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startAP) Access Point started. SSID: "));
                buf.add(apSsid);
                buf.add(F(", Password: "));
                buf.add(apPassword);
                buf.add(F(", IP Address: "));
                buf.add(apIP.toString().c_str());
                return buf; });

    return true;
}

void NetworkManager::setMdnsName(const char *mdnsName)
{
    this->mdnsName = mdnsName;
}

void NetworkManager::setWifiConnectionTimeout(unsigned long timeout)
{
    this->wifiConnectionTimeout = timeout;
}

bool NetworkManager::stopMDNS()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::stopMDNS) Attempting to stop mDNS responder..."));
                return buf; });

    bool status = MDNS.end();

    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::stopMDNS) mDNS responder stopped successfully.")); return buf; });
        return true;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf.add(F("(NetworkManager::stopMDNS) Failed to stop mDNS responder.")); return buf; });

    return false;
}

bool NetworkManager::startMDNS()
{
    return startMDNS(mdnsName);
}

bool NetworkManager::startMDNS(const String32 &mdnsName)
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startMDNS) Attempting to start mDNS responder with name: "));
                buf.add(mdnsName);
                return buf; });

    if (mdnsName.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::startMDNS) Failed to start mDNS responder: mDNS name is empty."));
                    return buf; });
        return false;
    }

    bool status = MDNS.begin(mdnsName);
    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(NetworkManager::startMDNS) mDNS responder started successfully with name: "); buf.add(mdnsName); return buf; });
        return true;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::startMDNS) Failed to start mDNS responder with name: "); buf.add(mdnsName); return buf; });

    return false;
}

bool NetworkManager::stopAP()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::stopAP) Attempting to stop Wi-Fi Access Point..."));
                return buf; });

    bool status = WiFi.softAPdisconnect(true);

    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(NetworkManager::stopAP) Wi-Fi Access Point stopped."); return buf; });
        return true;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::stopAP) Wi-Fi Access Point was not stopped."); return buf; });

    return false;
}

bool NetworkManager::startDNS()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::startDNS) Attempting to start DNS responder"));
                return buf; });

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.setTTL(0);

    bool status = dnsServer.start(53, "*", WiFi.softAPIP(), WiFi.softAPIP());
    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(NetworkManager::startDNS) DNS responder started successfully."); return buf; });
        return true;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::startDNS) Failed to start DNS responder"); return buf; });

    return false;
}

bool NetworkManager::stopDNS()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::stopDNS) Attempting to stop DNS responder"));
                return buf; });

    dnsServer.stop();
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::stopDNS) DNS responder stopped."); return buf; });
    return true;
}

std::vector<WifiNetwork> NetworkManager::scanWifiNetworks()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::scanWifiNetworks) Starting Wi-Fi scan..."));
                return buf; });

    int networksFound = WiFi.scanNetworks();
    if (networksFound <= 0)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::scanWifiNetworks) No Wi-Fi networks found."));
                    return buf; });
        return {};
    }

    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::scanWifiNetworks) Found "));
                buf.add(networksFound);
                buf.add(F(" Wi-Fi networks."));
                return buf; });

    std::vector<WifiNetwork> networks;
    networks.reserve(networksFound);

    for (int i = 0; i < networksFound; i++)
    {
        WifiNetwork newNetwork(
            WiFi.SSID(i).c_str(),
            "",
            WiFi.RSSI(i),
            WiFi.encryptionType(i),
            WiFi.channel(i),
            WiFi.BSSIDstr(i).c_str(),
            WiFi.isHidden(i));

        networks.push_back(std::move(newNetwork));

        logger.log(LOG_DEBUG, [&i, &networks]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::scanWifiNetworks) SSID: "));
                    buf.add(networks[i].ssid);
                    buf.add(F(", RSSI: "));
                    buf.add(networks[i].rssi);
                    buf.add(F(", EncryptionType: "));
                    buf.add(networks[i].encryptionType);
                    buf.add(F(", Channel: "));
                    buf.add(networks[i].channel);
                    buf.add(F(", BSSID: "));
                    buf.add(networks[i].bssid);
                    buf.add(F(", Hidden: "));
                    buf.add(networks[i].hidden ? F("true") : F("false"));
                    return buf; });
    }
    WiFi.scanDelete();
    return networks;
}

bool NetworkManager::scanWifiNetworksAsync()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
        String128 buf;
        buf.add(F("(NetworkManager::scanWifiNetworksAsync) Starting asynchronous Wi-Fi scan..."));
        return buf; });

    int result = WiFi.scanNetworks(true);

    if (static_cast<ScanState>(result) == ScanState::RUNNING)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {
            String128 buf;
            buf.add(F("(NetworkManager::scanWifiNetworksAsync) Asynchronous Wi-Fi scan started."));
            return buf; });
        return true;
    }

    logger.log(LOG_ERROR, [&]() -> String128
               {
        String128 buf;
        buf.add(F("(NetworkManager::scanWifiNetworksAsync) Failed to start. Error code: "));
        buf.add(result);
        return buf; });

    return false;
}

std::vector<WifiNetwork> NetworkManager::getScanWifiNetworksAsyncResults()
{
    int networksFound = WiFi.scanComplete();
    if (networksFound <= 0)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::getScanWifiNetworksAsyncResults) Asynchronous Wi-Fi scan not found or scan not completed yet."));
                    return buf; });
        return {};
    }

    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::getScanWifiNetworksAsyncResults) Asynchronous Wi-Fi scan completed. Found "));
                buf.add(networksFound);
                buf.add(F(" Wi-Fi networks."));
                return buf; });

    std::vector<WifiNetwork> networks;
    networks.reserve(networksFound);

    for (int i = 0; i < networksFound; i++)
    {
        WifiNetwork newNetwork(
            WiFi.SSID(i).c_str(),
            "",
            WiFi.RSSI(i),
            WiFi.encryptionType(i),
            WiFi.channel(i),
            WiFi.BSSIDstr(i).c_str(),
            WiFi.isHidden(i));

        networks.push_back(std::move(newNetwork));

        logger.log(LOG_DEBUG, [&i, &networks]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::getScanWifiNetworksAsyncResults) SSID: "));
                    buf.add(networks[i].ssid);
                    buf.add(F(", RSSI: "));
                    buf.add(networks[i].rssi);
                    buf.add(F(", EncryptionType: "));
                    buf.add(networks[i].encryptionType);
                    buf.add(F(", Channel: "));
                    buf.add(networks[i].channel);
                    buf.add(F(", BSSID: "));
                    buf.add(networks[i].bssid);
                    buf.add(F(", Hidden: "));
                    buf.add(networks[i].hidden ? F("true") : F("false"));
                    return buf; });
    }
    WiFi.scanDelete();
    return networks;
}

StringN<18> NetworkManager::getMacAddress() const
{
    return WiFi.macAddress().c_str();
}

IPAddress NetworkManager::getAPIpAddress() const
{
    return WiFi.softAPIP();
}

const char *NetworkManager::getSsid() const
{
    return ssid;
}

const char *NetworkManager::getPass() const
{
    return password;
}

const char *NetworkManager::getMdnsName() const
{
    return mdnsName;
}

unsigned long NetworkManager::getWifiConnectionTimeout() const
{
    return wifiConnectionTimeout;
}

bool NetworkManager::tryConnectWifi(const String32 &ssid, const String32 &password)
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::tryConnectWifi) Attempting to connect to SSID: '"));
                buf.add(ssid);
                buf.add(F("'  PASS: '"));
                buf.add(password);
                buf.add(F("'"));
                return buf; });

    if (ssid.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::tryConnectWifi) Failed to connect to Wi-Fi network: SSID is empty.")); return buf; });
        return false;
    }

    WiFi.begin(ssid, password);
    const unsigned long startTime = millis();

    while (getStatusWifi() != ConnState::WL_CONNECTED && millis() - startTime < WIFI_CONNECTION_TIMEOUT_MS)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::tryConnectWifi) Connecting to Wi-Fi...")); return buf; });
        delay(500);
    }

    if (getStatusWifi() == ConnState::WL_CONNECTED)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::tryConnectWifi) Successfully connected to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("' with IP: "));
                    buf.add(WiFi.localIP().toString().c_str());
                    return buf; });

        configureWifiPerformance();
        return true;
    }
    else
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::tryConnectWifi) Failed to connect to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("'"));
                    return buf; });

        WiFi.disconnect(true, true);
        return false;
    }
}

bool NetworkManager::tryConnectWifiAsync(const String32 &ssid, const String32 &password)
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::tryConnectWifiAsync) Attempting to connect to SSID: '"));
                buf.add(ssid);
                buf.add(F("'  PASS: '"));
                buf.add(password);
                buf.add(F("'"));
                return buf; });

    if (ssid.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::tryConnectAsyncWifi) Failed to connect to Wi-Fi network: SSID is empty.")); return buf; });
        return false;
    }

    WiFi.begin(ssid, password);

    logger.log(LOG_INFO, [&]() -> String128
               {
        String128 buf;
        buf.add(F("(NetworkManager::tryConnectWifiAsync) Async connection started"));
        return buf; });

    onGotIpHandlers[0] = WiFi.onStationModeGotIP([this, ssid](const WiFiEventStationModeGotIP &event)
                                                 {
        logger.log(LOG_INFO, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(NetworkManager::tryConnectWifiAsync) Successfully connected to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("' with IP: "));
                    buf.add(WiFi.localIP().toString().c_str());
                    return buf; });

        configureWifiPerformance();
        this->onGotIpHandlers[0] = nullptr; 
        this->onDisconnectedHandlers[0] = nullptr; });

    onDisconnectedHandlers[0] = WiFi.onStationModeDisconnected([this, ssid](const WiFiEventStationModeDisconnected &event)
                                                               {
                                                                   // switch(static_cast<WifiFailState>(event.reason))
                                                                   // {
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_NO_AP_FOUND:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_AUTH_FAIL:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_ASSOC_FAIL:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT:
                                                                   //     case WifiFailState::WIFI_DISCONNECT_REASON_BEACON_TIMEOUT:

                                                                   logger.log(LOG_WARN, [&]() -> String128
                                                                              {
                    String128 buf;
            buf.add(F("(NetworkManager::tryConnectWifiAsync) Failed to connect to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("'"));
                    buf.add(F(" Reason: "));
                    buf.add(event.reason);
                    return buf; });

                                                                   WiFi.disconnect(true, true);
                                                                   this->onGotIpHandlers[0] = nullptr;
                                                                   this->onDisconnectedHandlers[0] = nullptr;

                                                                   // break;
                                                                   // }
                                                               });

    return true;
}
