#include "NetworkManager.h"

NetworkManager &NetworkManager::init(Logger &logger, const char* apSsid, const char* apPassword, const char* mdnsName)
{
    static NetworkManager instance(logger, apSsid, apPassword, mdnsName);
    return instance;
}

NetworkManager::NetworkManager(Logger &logger, const char* apSsid, const char* apPassword, const char* mdnsName)
    : logger(logger),
      webserver(logger, 80),
      apSsid(apSsid),
      apPassword(apPassword),
      ssid(F("")),
      password(F("")),
      attemptSsid(F("")),
      attemptPassword(F("")),
      mdnsName(mdnsName),
      onGotIpHandlers{},
      onDisconnectedHandlers{}
{
}

ConnState NetworkManager::statusWifi()
{
    return static_cast<ConnState>(WiFi.status());
}

ScanState NetworkManager::statusScan() {
    int result = WiFi.scanComplete();

    if (result >= 0) {
        return ScanState::COMPLETED; 
    }

    return static_cast<ScanState>(result);
}

bool NetworkManager::begin()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::begin) Starting Network Manager..."); return buf; });

    if (attemptConnection(ssid, password))
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(NetworkManager::begin) Network Manager finished successfully."); return buf; });
        return true;
    }

    if (!startAP(apSsid, apPassword))
    {
        logger.log(LOG_ERROR, [&]() -> String128
                   { String128 buf;  buf = F("(NetworkManager::begin) Failed to start Access Point. Setup aborted."); return buf; });
        return false;
    }

    startMDNS(mdnsName);
    unsigned long lastWifiReconnectAttempt = millis();
    while (statusWifi() != ConnState::WL_CONNECTED || webserver.isRunning())
    {   
        unsigned long now = millis();
        if (statusWifi() == ConnState::WL_CONNECTED &&
            now - webserver.getLastRequestTime() >= WIFI_AFTER_WEBSERVER_IDLE_MS) 
        {
            
            webserver.stop();
            break;
        }

        if (statusWifi() != ConnState::WL_CONNECTED &&
            now - webserver.getLastRequestTime() >= WIFI_AFTER_WEBSERVER_IDLE_MS &&
            now - lastWifiReconnectAttempt >= WIFI_RETRY_DELAY_MS) 
        {   
            logger.log(LOG_INFO, [&]() -> String128 
                        { String128 buf; buf = F("(NetworkManager::begin) Connect async background Wi-Fi reconnect (webserver idle)..."); return buf; }); 

            attemptConnectionAsync(ssid, password);
            lastWifiReconnectAttempt = now;
        }

        if (!webserver.isRunning() && statusWifi() != ConnState::WL_CONNECTED) 
        {
            initServer();
        }

        webserver.handleClient();
        MDNS.update();
        esp_yield();
    }

    stopMDNS();
    stopAP();
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(NetworkManager::begin) Wi-Fi setup finished successfully."); return buf; });

    return true;
}

void NetworkManager::initServer()
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(NetworkManager::initServer) Initializing web server..."));
                return buf; });

    webserver.begin([this]()
                    { return this->scanWifiNetworksAsync(); },
                    [this]()
                    { return this->statusScan(); },
                    [this]()
                    { return this->getScanWifiNetworksAsyncResults(); },
                    [this](const char* ssid, const char* pass)
                    { return this->attemptConnectionAsync(ssid, pass); },
                    [this]()
                    { return this->statusWifi(); });
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

bool NetworkManager::attemptConnectionAsync(const char *ssid, const char *password)
{   
    for (int i = 0; i < MAX_WIFI_HANDLER; i++) 
    {
        this->onGotIpHandlers[i] = nullptr;
        this->onDisconnectedHandlers[i] = nullptr;
    }

    this->attemptSsid = ssid;
    this->attemptPassword = password;
    
    if(!tryConnectWifiAsync(attemptSsid, attemptPassword)){
        this->attemptSsid = F("");
        this->attemptPassword = F("");
        return false;
    }

    onGotIpHandlers[1] = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event) {
        this->ssid = this->attemptSsid;
        this->password = this->attemptPassword;
        this->attemptSsid = F("");
        this->attemptPassword = F("");
        this->onGotIpHandlers[1] = nullptr; 
        this->onDisconnectedHandlers[1] = nullptr;
    });

    onDisconnectedHandlers[1] = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event) {
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

void NetworkManager::setWiFiConfig(const char *ssid, const char *password)
{
    this->ssid = ssid;
    this->password = password;
}

void NetworkManager::setAPConfig(const char *apSsid, const char *apPassword)
{
    this->apSsid = apSsid;
    this->apPassword = apPassword;
}

bool NetworkManager::startAP(const String32 &apSsid, const String32 &apPassword)
{
    logger.log(LOG_INFO, [&]() -> String128
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

bool NetworkManager::stopMDNS()
{
    bool status = MDNS.end();

    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::stopMDNS) mDNS responder stopped successfully.")); return buf; });
    }
    else
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::stopMDNS) Failed to stop mDNS responder.")); return buf; });
    }

    return status;
}

bool NetworkManager::startMDNS(const String32 &mdnsName)
{
    bool status = MDNS.begin(mdnsName);
    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(NetworkManager::startMDNS) mDNS responder started successfully with name: "); buf.add(mdnsName); return buf; });
        return status;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::startMDNS) Failed to start mDNS responder with name: "); buf.add(mdnsName); return buf; });

    return status;
}

bool NetworkManager::stopAP()
{
    bool status = WiFi.softAPdisconnect(true);
    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(NetworkManager::stopAP) Wi-Fi Access Point stopped."); return buf; });
        return status;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(NetworkManager::stopAP) Wi-Fi Access Point was not stopped."); return buf; });

    return status;
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

bool NetworkManager::scanWifiNetworksAsync() {
    logger.log(LOG_DEBUG, [&]() -> String128 {
        String128 buf;
        buf.add(F("(NetworkManager::scanWifiNetworksAsync) Starting asynchronous Wi-Fi scan..."));
        return buf;
    });

    int result = WiFi.scanNetworks(true);

    if (static_cast<ScanState>(result) == ScanState::RUNNING) {
        logger.log(LOG_DEBUG, [&]() -> String128 {
            String128 buf;
            buf.add(F("(NetworkManager::scanWifiNetworksAsync) Asynchronous Wi-Fi scan started."));
            return buf;
        });
        return true;
    }

    logger.log(LOG_ERROR, [&]() -> String128 {
        String128 buf;
        buf.add(F("(NetworkManager::scanWifiNetworksAsync) Failed to start. Error code: "));
        buf.add(result);
        return buf;
    });

    return false;
}

std::vector<WifiNetwork> NetworkManager::getScanWifiNetworksAsyncResults() {
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

const char *NetworkManager::getSsid() const
{
    return ssid;
}

const char *NetworkManager::getPass() const
{
    return password;
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

    while (statusWifi() != ConnState::WL_CONNECTED && millis() - startTime < WIFI_CONNECTION_TIMEOUT_MS)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(NetworkManager::tryConnectWifi) Connecting to Wi-Fi...")); return buf; });
        delay(500);
    }

    if (statusWifi() == ConnState::WL_CONNECTED)
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

        WiFi.enableSTA(false);
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

    logger.log(LOG_INFO, [&]() -> String128 {
        String128 buf;
        buf.add(F("(NetworkManager::tryConnectWifiAsync) Async connection started"));
        return buf;
    });

    onGotIpHandlers[0] = WiFi.onStationModeGotIP([this, ssid](const WiFiEventStationModeGotIP& event) {
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
        this->onDisconnectedHandlers[0] = nullptr;
    });

    onDisconnectedHandlers[0] = WiFi.onStationModeDisconnected([this, ssid](const WiFiEventStationModeDisconnected& event) {
        
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
                    
            WiFi.enableSTA(false);
            this->onGotIpHandlers[0] = nullptr;
            this->onDisconnectedHandlers[0] = nullptr;

            // break;
        // }
    });

    return true;
}
