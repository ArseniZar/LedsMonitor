#include "WiFiSetup.h"

WiFiSetup::WiFiSetup(Logger &logger)
    : _logger(logger) {}

bool WiFiSetup::status()
{
    bool connected = (WiFi.status() == WL_CONNECTED);
    _connState = connected ? CONNECTED : WAITING;
    return connected;
}

void WiFiSetup::begin(EEPROMSetup &_eeprom, const String &_apSSID, const String &_apPassword)
{
    _logger.log("Checking validity of saved MAC address in EEPROM...", LOG_INFO);
    saveMAC(_eeprom);
    
    _logger.log("Attempting to connect using credentials from EEPROM...", LOG_INFO);
    if (tryConnectFromEEPROM(_eeprom))
    {
        _logger.log("Wi-Fi connection established successfully from EEPROM credentials.", LOG_INFO);
        _connState = CONNECTED;
        return;
    }
    _logger.log("Failed to connect using EEPROM credentials.", LOG_WARN);
    _logger.log("Scanning for available Wi-Fi networks...", LOG_INFO);
    scanNetworks();
    _logger.log("Starting Wi-Fi Access Point mode for configuration.", LOG_WARN);
    if (startAP(_apSSID, _apPassword))
    {
        setupServer(_eeprom);
    }
    while (_connState != CONNECTED)
    {
        handle();
        yield();
    }
    stopServer();
    stopAP();
}

bool WiFiSetup::startAP(const String _apSSID, const String _apPassword)
{
    WiFi.mode(WIFI_AP_STA);
    if (!WiFi.softAP(_apSSID, _apPassword))
    {
        _logger.log("Failed to start Wi-Fi Access Point. Please check configuration.", LOG_ERROR);
        return false;
    }
    _logger.log("Access Point started. SSID: " + _apSSID + ", IP Address: " + WiFi.softAPIP().toString(), LOG_INFO);
    return true;
}

bool WiFiSetup::stopAP()
{
    WiFi.softAPdisconnect(true);
    _logger.log("Wi-Fi Access Point stopped.", LOG_WARN);
    WiFi.mode(WIFI_STA);
    return true;
}

String WiFiSetup::scanNetworks()
{
    int n = WiFi.scanNetworks();
    String _networksHtml = "<select name='ssid'>";
    for (int i = 0; i < n; i++)
    {
        String ssid = WiFi.SSID(i);
        _networksHtml += "<option value='" + ssid + "'>" + ssid + "</option>";
    }
    return _networksHtml += "</select>";
}

String WiFiSetup::decodeHtmlEntities(String input)
{
    String output = "";
    int len = input.length();
    for (int i = 0; i < len; i++)
    {
        if (input.charAt(i) == '&' && input.charAt(i + 1) == '#')
        {
            int semicolonPos = input.indexOf(';', i);
            if (semicolonPos > i)
            {
                String codeStr = input.substring(i + 2, semicolonPos);
                int code = codeStr.toInt();
                output += (char)code;
                i = semicolonPos;
                continue;
            }
        }
        output += input.charAt(i);
    }
    return output;
}

void WiFiSetup::setupServer(EEPROMSetup &_eeprom)
{
    _server.on("/", HTTP_GET, [this]()
               {
        String page = "<html><body>";
        page += "<h1>Wi-Fi Setup</h1>";
        
        page += "<form method='GET' action='/' style='margin-bottom:20px;'>";
        page += "<input type='submit' value='Refresh Networks'>";
        page += "</form>";

        page += "<form method='POST' action='/'>";
        page += "<label>Network:</label><br>";
        page += this->scanNetworks(); 
        page += "<br><label>Password:</label><br>";
        page += "<input type='password' name='password'><br><br>";
        page += "<input type='submit' value='Connect'>";
        page += "</form></body></html>";
        _server.send(200, "text/html", page); });

    _server.on("/", HTTP_POST, [this]()
               {
        String ssid = _server.arg("ssid");
        ssid.trim();
        _pendingSSID = ssid;

        String password = _server.arg("password");
        password.trim();
        
        _pendingPassword = WiFiSetup::decodeHtmlEntities(password);

        _connState = CONNECTING;

        _logger.log("User requested connection to SSID: '" + _pendingSSID + "'", LOG_INFO);
        _logger.log("Password entered: '" + _pendingPassword + "'", LOG_INFO);

        String page = "<html><body>";
        page += "<h2>Connecting to " + _pendingSSID + "...</h2>";
        page += "<div id='status'>Connecting...</div>";
        page += "<script>";
        page += "setInterval(function() { ";
        page += "fetch('/status').then(r => r.text()).then(t => { ";
        page += "document.getElementById('status').innerText = t; });";
        page += "}, 3000);";
        page += "</script>";
        page += "<a href='/'>Back</a>";
        page += "</body></html>";

        _server.send(200, "text/html", page); });

    _server.on("/status", HTTP_GET, [this, &_eeprom]()
               {
        if (_connState == CONNECTING) {
            if (tryConnect(_pendingSSID, _pendingPassword)) {
                _eeprom.writeString(_pendingSSID, ADDR_SSID);
                _eeprom.writeString(_pendingPassword, ADDR_PASS);
                _connState = CONNECTED;
                _logger.log("Wi-Fi connection established and credentials saved to EEPROM.", LOG_INFO);
            } else {
                _connState = FAILED;
                _logger.log("Wi-Fi connection attempt failed. Please check SSID and password.", LOG_WARN);
            }
        }

        String response;
        switch (_connState) {
            case WAITING: response = "Idle"; break;
            case CONNECTING: response = "Connecting..."; break;
            case CONNECTED: response = "Connected successfully."; break;
            case FAILED: response = "Connection failed."; break;
        }

        _server.send(200, "text/plain", response); });

    _server.begin();
    _logger.log("HTTP configuration server started and ready for connections.", LOG_INFO);
}

void WiFiSetup::stopServer()
{
    _server.close();
    _server.stop();
}

void WiFiSetup::handle()
{
    _server.handleClient();
}

bool WiFiSetup::tryConnectFromEEPROM(EEPROMSetup &eeprom)
{
    String savedSSID = eeprom.readString(ADDR_SSID);
    String savedPASS = eeprom.readString(ADDR_PASS);
    _logger.log("Loaded SSID from EEPROM: '" + savedSSID + "'", LOG_INFO);
    _logger.log("Loaded password from EEPROM: '" + savedPASS + "'", LOG_INFO);
    return tryConnect(savedSSID, savedPASS);
}

void WiFiSetup::saveMAC(EEPROMSetup &eeprom)
{
    String savedMAC = eeprom.readString(ADDR_MAC);
    String mac = WiFi.macAddress();
    if (mac.equals(savedMAC))
    {
        _logger.log("MAC address in EEPROM is valid", LOG_INFO);
        return;
    }
    else
    {
        eeprom.writeString(mac, ADDR_MAC);
        _logger.log("MAC address in EEPROM updated", LOG_WARN);
    }
}

bool WiFiSetup::tryConnect(const String SSID, const String PASS)
{
    String enteredSSID = SSID;
    String enteredPassword = PASS;

    if (enteredSSID.length() == 0)
        return false;

    _logger.log("Attempting to connect to SSID: '" + enteredSSID + "'", LOG_INFO);

    WiFi.begin(enteredSSID, enteredPassword);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
    {
        delay(500);
        _logger.log("Connecting to Wi-Fi...", LOG_INFO);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        _logger.log("Successfully connected to Wi-Fi network: '" + enteredSSID + "'", LOG_INFO);
        return true;
    }
    else
    {
        _logger.log("Failed to connect to Wi-Fi network: '" + enteredSSID + "'", LOG_WARN);
        return false;
    }
}
