#pragma once
#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#include <Arduino.h>
#include "MacAddress.h"

class DeviceBase
{
public:
    virtual ~DeviceBase() = default;
    const MacAddress& getMac() const { return mac; }

protected:
    const MacAddress &mac;
    DeviceBase() = delete;  
    DeviceBase(const MacAddress &mac): mac(mac) {}
};

#endif // DEVICE_BASE_H