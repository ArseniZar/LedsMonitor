#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "EEPROMSetup.h"
#include "config.h"
#include "Logger.h"

class WiFiSetup
{
public:
    WiFiSetup(Logger &logger);
    void begin(EEPROMSetup &_eeprom, const String &_apSSID, const String &_apPassword);
    bool tryConnect(const String SSID, const String PASS);
    bool status();

private:
    Logger &_logger;
    ESP8266WebServer _server = ESP8266WebServer(80);

    enum ConnState
    {
        WAITING,
        CONNECTING,
        CONNECTED,
        FAILED
    };
    ConnState _connState = WAITING;

    String _pendingSSID;
    String _pendingPassword;

    void handle();
    bool startAP(const String _apSSID, const String _apPassword);
    bool stopAP();

    void setupServer(EEPROMSetup &_eeprom);
    void stopServer();

    static String decodeHtmlEntities(String input);
    String scanNetworks();
    bool tryConnectFromEEPROM(EEPROMSetup &eeprom);

    void saveMAC(EEPROMSetup &eeprom);
};

#endif // WIFI_SETUP_H
