#pragma once
#include "DeviceLed.h"
// DeviceLed.tpp
template <typename T, typename E>
DeviceLed<T, E>::DeviceLed(Logger &logger, const MacAddress &mac, const String &name, uint16_t countLed, uint8_t pin) : DeviceBase(mac, name),
                                                                                                                       logger(logger),
                                                                                                                       device(countLed, pin),
                                                                                                                       countLed(countLed),
                                                                                                                       brightness(255),
                                                                                                                       color(Colors::WHITE),
                                                                                                                       status(false) {}
template <typename T, typename E>
void DeviceLed<T, E>::begin()
{
    device.Begin();
    setBrightness(brightness);
    setPower(status);
}

template <typename T, typename E>
void DeviceLed<T, E>::setBrightness(int newBrightness)
{
    if (newBrightness < 0)
        newBrightness = 0;
    if (newBrightness > 255)
        newBrightness = 255;

    brightness = newBrightness;
}

template <typename T, typename E>
RgbColor DeviceLed<T, E>::brightnessColor()
{
    return RgbColor(
        (uint8_t)(color.R * brightness / 255),
        (uint8_t)(color.G * brightness / 255),
        (uint8_t)(color.B * brightness / 255));
}

template <typename T, typename E>
void DeviceLed<T, E>::setPower(bool newStatus)
{
    device.ClearTo(newStatus ? brightnessColor() : Colors::BLACK);
    device.Show();
    this->status = newStatus;
}

template <typename T, typename E>
void DeviceLed<T, E>::setColor(const String &color)
{
    RgbColor hexColor = stringToRgbColor(color);
    this->color = hexColor;
    device.ClearTo(brightnessColor());
    device.Show();
    status = true;
}

template <typename T, typename E>
RgbColor DeviceLed<T, E>::stringToRgbColor(const String &colorStr)
{
    String hex = colorStr;
    if (hex.startsWith("#"))
    {
        hex = hex.substring(1);
    }

    if (hex.length() != 6)
    {
        return RgbColor(0, 0, 0);
    }

    char buf[3] = {0};
    buf[0] = hex[0];
    buf[1] = hex[1];
    uint8_t r = static_cast<uint8_t>(strtoul(buf, nullptr, 16));

    buf[0] = hex[2];
    buf[1] = hex[3];
    uint8_t g = static_cast<uint8_t>(strtoul(buf, nullptr, 16));

    buf[0] = hex[4];
    buf[1] = hex[5];
    uint8_t b = static_cast<uint8_t>(strtoul(buf, nullptr, 16));

    return RgbColor(r, g, b);
}