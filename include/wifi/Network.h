#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <GSON.h>
#include "JsonSerializable.h"
#include "JsonParse.h"
#include "JsonTypes.h"


class Network: public JsonSerializable, JsonParse
{
private:

    enum class EncryptionType
    {
        OPEN = 7,   // ENC_TYPE_NONE
        WEP = 5,    // ENC_TYPE_WEP
        WPA = 2,    // ENC_TYPE_TKIP
        WPA2 = 4,   // ENC_TYPE_CCMP
        UNKNOWN = 8 // ENC_TYPE_AUTO
    };
    EncryptionType toEncryptionType(int enc);


public:
    String ssid;
    String password;
    int rssi;
    EncryptionType encryptionType;
    int channel;
    String bssid;
    bool hidden;

    Network() = default;
    
    explicit Network(const String &ssid,
                     const String &password,
                     int rssi = 0,
                     int encryptionType = 8,
                     int channel = 0,
                     const String &bssid = "",
                     bool hidden = false);

    Network(const Network &other);

    gson::Str toJson() const override;
    static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &e);
};

#endif // NETWORK_H
