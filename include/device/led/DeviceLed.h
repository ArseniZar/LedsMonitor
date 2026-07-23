#pragma once
#ifndef DEVICE_LED_H
#define DEVICE_LED_H

#include <NeoPixelBus.h>
#include <memory>
#include "Logger.h"
#include "DeviceBase.h"
#include "DeviceLedConfigModels.h"

#ifndef DEVICE_LED_NAME
#define DEVICE_LED_NAME "Led"
#endif

#ifndef DEVICE_LED_BRIGHTNESS
#define DEVICE_LED_BRIGHTNESS 255
#endif

namespace Colors
{
    inline const RgbColor RED = RgbColor(0xFF, 0x00, 0x00);
    inline const RgbColor GREEN = RgbColor(0x00, 0xFF, 0x00);
    inline const RgbColor BLUE = RgbColor(0x00, 0x00, 0xFF);
    inline const RgbColor WHITE = RgbColor(0xFF, 0xFF, 0xFF);
    inline const RgbColor YELLOW = RgbColor(0xFF, 0xFF, 0x00);
    inline const RgbColor CYAN = RgbColor(0x00, 0xFF, 0xFF);
    inline const RgbColor MAGENTA = RgbColor(0xFF, 0x00, 0xFF);
    inline const RgbColor BLACK = RgbColor(0x00, 0x00, 0x00);
    inline const RgbColor ORANGE = RgbColor(0xFF, 0xA5, 0x00);
    inline const RgbColor PURPLE = RgbColor(0x80, 0x00, 0x80);
    inline const RgbColor PINK = RgbColor(0xFF, 0xC0, 0xCB);
    inline const RgbColor GREY = RgbColor(0x80, 0x80, 0x80);
}

template <typename T, typename E>
class DeviceLed final : public DeviceBase
{
public:
    DeviceLed(Logger &logger, const MacAddress &mac, uint8_t pin, uint16_t countLed);
    void begin();
    void setCountLed(uint16_t countLed);
    void setPower(bool status);
    void setBrightness(int  brightness);
    void setColor(const char *color);
    void applyConfig(const DeviceLedConfig &config);
    StringN<8> getColor();
    bool getStatus();

private:
    Logger &logger;
    NeoPixelBus<T, E> device;
    //TODO make gamma correction configurable https://github.com/Makuna/NeoPixelBus/wiki/T_GAMMA
    const uint8_t pin;
    const uint16_t countLed;

    int brightness;
    RgbColor color;
    bool status;

    
    RgbColor stringHexToRgbColor(const char *colorStr);
    StringN<8> rgbColorToStringHex(const RgbColor& c);
    RgbColor brightnessColor();
};

#include "DeviceLed.tpp"
#endif // DEVICE_LED_H