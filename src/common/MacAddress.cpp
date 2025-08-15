#include "MacAddress.h"

MacAddress::MacAddress(const String &macAddr, const String &bcast) : address(normalize(macAddr)), broadcast(normalize(bcast)) {}

MacAddress &MacAddress::init(const String &macAddr, const String &bcast)
{
    static MacAddress instance(macAddr, bcast);
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