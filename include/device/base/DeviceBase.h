#pragma once
#ifndef DEVICE_BASE_H
#define DEVICE_BASE_H

#include <Arduino.h>
#include <StringN.h>
#include "MacAddress.h"

class DeviceBase {
public:
    virtual ~DeviceBase() = default;
    
    const MacAddress& getMacAddress();
    const char* getName() const;
    void setName(const char* name);
    
protected:
    const MacAddress& mac;
    String32 name;

    DeviceBase() = delete;
    DeviceBase(const MacAddress& mac, const char* name);
};

#endif // DEVICE_BASE_H