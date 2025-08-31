#pragma once
#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <Arduino.h>

class MacAddress
{
public:
    static MacAddress &init(const String &macAddr);
    bool isBroadcast(const String &mac) const;
    bool equals(const String &mac) const;
    String getMac() const;

private:
    const String address;
    const String broadcast;
    static String normalize(const String &mac);
    MacAddress(const String &macAddr);
    MacAddress() = delete;
};

#endif // MAC_ADDRESS_H
