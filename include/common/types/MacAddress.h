#pragma once
#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <Arduino.h>
#include <StringN.h>

class MacAddress
{
public:
    static MacAddress &init(const char *macAddr);
    bool isBroadcast(const char *mac) const;
    bool equals(const char *mac) const;
    const char *getMac() const;

private:
    const StringN<18> address;
    const StringN<18> broadcast;
    StringN<18> normalize(const char *mac) const;
    MacAddress(const char *macAddr);
    MacAddress() = delete;
};

#endif // MAC_ADDRESS_H
