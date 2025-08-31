#include "MacAddress.h"

MacAddress::MacAddress(const String &macAddr) : address(normalize(macAddr)), broadcast(normalize("FF:FF:FF:FF:FF:FF")) {}

MacAddress &MacAddress::init(const String &macAddr)
{
    static MacAddress instance(macAddr);
    return instance;
}

String MacAddress::normalize(const String &mac)
{
    String lower = mac;
    lower.trim();
    lower.toLowerCase();
    return lower;
}

bool MacAddress::isBroadcast(const String &mac) const
{
    return normalize(mac) == broadcast;
}

bool MacAddress::equals(const String &mac) const
{
    return normalize(mac) == address;
}

String MacAddress::getMac() const
{
    return address;
}