#include "DeviceLedConfigModels.h"

DeviceLedConfig *DeviceLedConfig::defaultInstance = nullptr;

DeviceLedConfig::DeviceLedConfig(const DeviceLedConfig &def)
    : pin(def.pin), countLed(def.countLed), deviceName(def.deviceName)
{
}

DeviceLedConfig::DeviceLedConfig(uint8_t p, uint16_t c, const char *name)
    : pin(p), countLed(c), deviceName(name)
{
}

const DeviceLedConfig &DeviceLedConfig::defaultConfig(uint8_t pin, uint16_t countLed, const char *deviceName)
{
    if (!defaultInstance)
    {
        defaultInstance = new DeviceLedConfig(pin, countLed, deviceName);
    }
    return *defaultInstance;
}

const DeviceLedConfig &DeviceLedConfig::fromDefault()
{
    return *defaultInstance;
}

void DeviceLedConfig::operator=(const DeviceLedRuntimeConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}

void DeviceLedConfig::operator=(const DeviceLedConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}

bool DeviceLedConfig::operator==(const DeviceLedRuntimeConfig &config) const
{
    return (countLed == config.countLed) &&
           (strcmp(deviceName.c_str(), config.deviceName.c_str()) == 0); //FIXME: Временный костыль из-за отсутствия operator== в StringN.
}

DeviceLedRuntimeConfig::DeviceLedRuntimeConfig(const DeviceLedConfig &config)
    : countLed(config.countLed), deviceName(config.deviceName)
{
}

void DeviceLedRuntimeConfig::operator=(const DeviceLedConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}
