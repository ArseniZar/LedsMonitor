#pragma once
#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <Arduino.h>

class MacAddress
{
public:
    static MacAddress &init(const String &macAddr, const String &bcast);
    bool isBroadcast(const String &mac) const;
    bool equals(const String &mac) const;


private:
    const String address;
    const String broadcast;
    static String normalize(const String &mac);
    MacAddress(const String &macAddr, const String &bcast);
    MacAddress() = delete;

};

#endif // MAC_ADDRESS_H
