#include "DeviceBase.h"

DeviceBase::DeviceBase(const MacAddress &mac, const char *name) : mac(mac), name(name) {}


const MacAddress &DeviceBase::getMacAddress()
{
    return mac;
}

const char *DeviceBase::getName() const
{
    return name.c_str();
}

void DeviceBase::setName(const char *newName)
{
    this->name = newName;
}