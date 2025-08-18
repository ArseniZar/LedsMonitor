#pragma once
#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#include <Arduino.h>
#include "MacAddress.h"

class DeviceBase
{
public:
    virtual ~DeviceBase() = default;
    const MacAddress &getMac() const { return mac; }
    const String getName() const { return name; }

protected:
    const MacAddress &mac;
    const String name;
    DeviceBase() = delete;
    DeviceBase(const MacAddress &mac, const String name) : mac(mac), name(name) {}
};

#endif // DEVICE_BASE_H