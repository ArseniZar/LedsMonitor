#pragma once
#ifndef DEVICE_LED_CONFIG_MODELS_H
#define DEVICE_LED_CONFIG_MODELS_H

#include <Arduino.h>
#include <StringN.h>

struct DeviceLedRuntimeConfig;

class DeviceLedConfig
{
public:
    const uint8_t pin;
    uint16_t countLed;
    String32 deviceName;

    static const DeviceLedConfig &defaultConfig(uint8_t pin, uint16_t countLed, const char *deviceName);

    static const DeviceLedConfig &fromDefault();
    static constexpr const char *TYPE_NAME = "DeviceLedConfig";

    DeviceLedConfig(const DeviceLedConfig &def);

    void operator=(const DeviceLedRuntimeConfig &config);
    void operator=(const DeviceLedConfig &config);
    bool operator==(const DeviceLedRuntimeConfig &config) const;

private:
    DeviceLedConfig(uint8_t p, uint16_t c, const char *name);
    static DeviceLedConfig *defaultInstance;
};

struct DeviceLedRuntimeConfig
{
    static constexpr const char *PATH = "/deviceLed";
    static constexpr uint8_t KEY = 'L';

    uint16_t countLed;
    String32 deviceName;
    DeviceLedRuntimeConfig() = default;
    DeviceLedRuntimeConfig(const DeviceLedConfig &config);
    void operator=(const DeviceLedConfig &config);
};

#endif // DEVICE_LED_CONFIG_MODELS_H