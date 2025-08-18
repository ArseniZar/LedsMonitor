#include "WiFiSetup.h"

WiFiSetup &WiFiSetup::init(Logger &logger)
{
    static WiFiSetup instance(logger);
    return instance;
}

WiFiSetup::WiFiSetup(Logger &logger)
    : logger(logger),
      webserver(logger, 80),
      apSsid("SmartHome"),
      apPassword("12345678"),
      ssid(""),
      password(""),
      mdnsName("smarthome")
{
}

ConnState WiFiSetup::status()
{
    return static_cast<ConnState>(WiFi.status());
}

bool WiFiSetup::begin()
{
    logger.log("(WiFiSetup::begin) Starting Wi-Fi setup...", LOG_INFO);

    if (attemptConnection(ssid, password))
    {
        logger.log("(WiFiSetup::begin) Wi-Fi suetup finished successfully.", LOG_INFO);
        return true;
    }

    if (!startAP(apSsid, apPassword))
    {
        logger.log("(WiFiSetup::begin) Failed to start Access Point. Setup aborted.", LOG_ERROR);
        return false;
    }

    startMDNS(mdnsName);

    while (status() != ConnState::WL_CONNECTED || webserver.isRunning())
    {

        if (!webserver.isRunning())
        {
            if (!initServer())
            {
                logger.log("(WiFiSetup::begin) Failed to initialize web server. Setup aborted.", LOG_ERROR);
                return false;
            }
        }

        webserver.handleClient();
        MDNS.update();

        if (can_yield())
            esp_yield();
    }

    stopAP();
    stopMDNS();
    logger.log("(WiFiSetup::begin) Wi-Fi suetup finished successfully.", LOG_INFO);
    return true;
}

bool WiFiSetup::initServer()
{
    logger.log("(WiFiSetup::initServer) Initializing web server...", LOG_INFO);

    bool serverStarted = webserver.begin(
        [this]()
        {
            return this->scanNetworks();
        },
        [this](const String &ssid, const String &pass)
        {
            return this->attemptConnection(ssid, pass);
        });

    logger.log("(WiFiSetup::initServer) Web server initialization " +
                   String(serverStarted ? "succeeded." : "failed."),
               serverStarted ? LOG_INFO : LOG_ERROR);

    return serverStarted;
}

bool WiFiSetup::attemptConnection(const String &ssid, const String &password)
{
    if (tryConnectWifi(ssid, password))
    {
        this->ssid = ssid;
        this->password = password;
        return true;
    }
    return false;
}

void WiFiSetup::setWiFiConfig(const String &ssid, const String &password)
{
    this->ssid = ssid;
    this->password = password;
}

void WiFiSetup::setAPConfig(const String &apSsid, const String &apPassword)
{
    this->apSsid = apSsid;
    this->apPassword = apPassword;
}

bool WiFiSetup::startAP(const String &apSsid, const String &apPassword)
{
    logger.log("(WiFiSetup::startAP) Attempting to start Wi-Fi Access Point with SSID: '" + apSsid + "'", LOG_INFO);
    if (apSsid.isEmpty())
    {
        logger.log("(WiFiSetup::startAP) Failed to start Access Point: SSID is empty.", LOG_WARN);
        return false;
    }

    WiFi.mode(WIFI_AP);

    bool phySet = WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    logger.log("(WiFiSetup::startAP) PHY mode set to WIFI_PHY_MODE_11G with 50 ms delay " +
                   String(phySet ? "enabled successfully" : "failed to enable"),
               phySet ? LOG_DEBUG : LOG_WARN);

    bool apStarted = WiFi.softAP(apSsid, apPassword);
    logger.log("(WiFiSetup::startAP) Starting Wi-Fi Access Point " +
                   String(apStarted ? "succeeded." : "failed. Please check configuration."),
               apStarted ? LOG_INFO : LOG_ERROR);

    if (!apStarted)
    {
        return false;
    }

    IPAddress apIP = WiFi.softAPIP();
    bool ipValid = (apIP != IPAddress(0, 0, 0, 0));
    logger.log("(WiFiSetup::startAP) AP IP address " +
                   String(ipValid ? "is valid: " + apIP.toString() : "is invalid after starting softAP!"),
               ipValid ? LOG_INFO : LOG_ERROR);

    if (!ipValid)
    {
        return false;
    }

    logger.log("(WiFiSetup::startAP) Access Point started. SSID: " + apSsid +
                   ", Password: " + apPassword +
                   ", IP Address: " + apIP.toString(),
               LOG_INFO);

    return true;
}

void WiFiSetup::setMdnsName(const String &mdnsName)
{
    this->mdnsName = mdnsName;
}

bool WiFiSetup::stopMDNS()
{
    bool status = MDNS.end();
    if (status)
    {
        logger.log("(WiFiSetup::stopMDNS) mDNS responder stopped successfully.", LOG_INFO);
        return status;
    }
    logger.log("(WiFiSetup::stopMDNS) Failed to stop mDNS responder.", LOG_WARN);
    return status;
}

bool WiFiSetup::startMDNS(const String &mdnsName)
{
    bool status = MDNS.begin(mdnsName);
    if (status)
    {
        logger.log("(WiFiSetup::startMDNS) mDNS responder started successfully with name: " + mdnsName, LOG_INFO);
        return status;
    }
    logger.log("(WiFiSetup::startMDNS) Failed to start mDNS responder with name: " + mdnsName, LOG_WARN);
    return status;
}

bool WiFiSetup::stopAP()
{
    bool status = WiFi.softAPdisconnect(true);
    if (!status)
    {
        logger.log("(WiFiSetup::stopAP) Wi-Fi Access Point stopped.", LOG_INFO);
        return status;
    }
    logger.log("(WiFiSetup::stopAP) Wi-Fi Access Point was not stopped.", LOG_WARN);
    return status;
}

std::vector<Network> WiFiSetup::scanNetworks()
{
    logger.log("(WiFiSetup::scanNetworks) Starting Wi-Fi scan...", LOG_DEBUG);
    std::vector<Network> networks;
    int networksFound = WiFi.scanNetworks();
    if (networksFound <= 0)
    {
        logger.log("(WiFiSetup::scanNetworks) No Wi-Fi networks found.", LOG_DEBUG);
        return networks;
    }

    logger.log("(WiFiSetup::scanNetworks) Found " + String(networksFound) + " Wi-Fi networks.", LOG_DEBUG);

    for (int i = 0; i < networksFound; i++)
    {
        Network newNetwork(
            WiFi.SSID(i),
            "",
            WiFi.RSSI(i),
            WiFi.encryptionType(i),
            WiFi.channel(i),
            WiFi.BSSIDstr(i),
            WiFi.isHidden(i));

        networks.push_back(newNetwork);
        logger.log(
            "(WiFiSetup::scanNetworks) SSID: " + newNetwork.ssid +
                ", RSSI: " + String(newNetwork.rssi) +
                ", Channel: " + String(newNetwork.channel) +
                ", BSSID: " + newNetwork.bssid +
                ", Hidden: " + String(newNetwork.hidden ? "true" : "false"),
            LOG_DEBUG);
    }
    WiFi.disconnect(true);
    return networks;
}

const String WiFiSetup::getMAC() const
{
    return WiFi.macAddress();
}

const String &WiFiSetup::getSsid() const
{
    return ssid;
}

const String &WiFiSetup::getPass() const
{
    return password;
}

bool WiFiSetup::tryConnectWifi(const String &ssid, const String &password)
{
    logger.log("(WiFiSetup::tryConnectWifi) Attempting to connect to SSID: '" + ssid + "'  PASS: '" + password + "'", LOG_INFO);
    if (ssid.isEmpty())
    {
        logger.log("(WiFiSetup::tryConnectWifi) Failed to connect to Wi-Fi network: SSID is empty.", LOG_WARN);
        return false;
    }
    WiFi.begin(ssid, password);

    const unsigned long startTime = millis();

    while (status() != ConnState::WL_CONNECTED && millis() - startTime < WIFI_CONNECTION_TIMEOUT_MS)
    {
        delay(500);
        logger.log("(WiFiSetup::tryConnectWifi) Connecting to Wi-Fi...", LOG_DEBUG);
    }

    if (status() == ConnState::WL_CONNECTED)
    {
        logger.log("(WiFiSetup::tryConnectWifi) Successfully connected to Wi-Fi network: '" + ssid +
                       "' with IP: " + WiFi.localIP().toString(),
                   LOG_INFO);
        bool sleepModeSet = WiFi.setSleepMode(WIFI_NONE_SLEEP, 50);
        logger.log("(WiFiSetup::tryConnectWifi) Wi-Fi sleep mode WIFI_NONE_SLEEP with 50 ms delay " +
                       String(sleepModeSet ? "enabled successfully" : "failed to enable"),
                   sleepModeSet ? LOG_DEBUG : LOG_WARN);

        WiFi.setOutputPower(20.5f);
        logger.log("(WiFiSetup::tryConnectWifi) Wi-Fi output power set to 20.5 dBm ", LOG_DEBUG);

        return true;
    }
    else
    {
        logger.log("(WiFiSetup::tryConnectWifi) Failed to connect to Wi-Fi network: '" + ssid + "'", LOG_WARN);
        WiFi.disconnect(true);
        return false;
    }
}
