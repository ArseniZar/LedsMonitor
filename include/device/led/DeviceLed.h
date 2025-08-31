#pragma once
#ifndef DEVICE_LED_H
#define DEVICE_LED_H

#include <NeoPixelBus.h>
#include "Logger.h"
#include "DeviceBase.h"

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
    DeviceLed(Logger &logger, const MacAddress &mac, const String &name, uint16_t countLed, uint8_t pin);
    void begin();
    void setPower(bool newStatus);
    void setBrightness(int newBrightness);
    void setColor(const String &color);

private:
    Logger &logger;
    NeoPixelBus<T, E> device;

    const int countLed;
    int brightness;
    RgbColor color;
    bool status;

    RgbColor stringToRgbColor(const String &colorStr);
    RgbColor brightnessColor();
};

#include "DeviceLed.tpp"
#endif // DEVICE_LED_H