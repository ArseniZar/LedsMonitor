#include "MacAddress.h"

MacAddress::MacAddress(const char *macAddr) : address(normalize(macAddr)), broadcast(normalize(StringN<18>(F("FF:FF:FF:FF:FF:FF")))) {}

MacAddress &MacAddress::init(const char *macAddr)
{
    static MacAddress instance(macAddr);
    return instance;
}

StringN<18> MacAddress::normalize(const char *mac) const
{
    StringN<18> result;
    size_t j = 0;
    for (size_t i = 0; mac[i] != '\0' && j < 17; ++i)
    {
        char c = mac[i];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            result[j++] = tolower(c);
        }
    }
    result[j] = '\0';
    return result;
}

bool MacAddress::isBroadcast(const char *mac) const
{
    return strcmp(normalize(mac).c_str(), broadcast.c_str()) == 0;
}

bool MacAddress::equals(const char *mac) const
{
    return strcmp(normalize(mac).c_str(), address.c_str()) == 0;
}

const char *MacAddress::getMac() const
{
    return address;
}