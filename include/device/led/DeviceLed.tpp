#pragma once
#include "DeviceLed.h"

template <typename T, typename E>
DeviceLed<T, E>::DeviceLed(Logger &logger, const MacAddress &mac, uint8_t pin, uint16_t countLed) : DeviceBase(mac, DEVICE_LED_NAME),
                                                                                 logger(logger),
                                                                                 device(countLed, pin),
                                                                                 pin(pin),
                                                                                 countLed(countLed),
                                                                                 brightness(DEVICE_LED_BRIGHTNESS),
                                                                                 color(Colors::WHITE),
                                                                                 status(true) {}
template <typename T, typename E>
void DeviceLed<T, E>::begin()
{
    device.Begin();
    setBrightness(brightness);
    setPower(status);
}

template <typename T, typename E>
void DeviceLed<T, E>::setBrightness(int brightness)
{
    if (brightness < 0)
        brightness = 0;
    if (brightness > 255)
        brightness = 255;

    this->brightness = brightness;
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
void DeviceLed<T, E>::setPower(bool status)
{
    this->status = status;
    device.ClearTo(this->status ? brightnessColor() : Colors::BLACK);
    device.Show();
}

template <typename T, typename E>
void DeviceLed<T, E>::setColor(const char *color)
{
    RgbColor hexColor = stringHexToRgbColor(color);
    this->color = std::move(hexColor);

    if (status)
    {
        device.ClearTo(brightnessColor());
        device.Show();
    }
}

template <typename T, typename E>
const char *DeviceLed<T, E>::getColor()
{
    return rgbColorToStringHex(color);
}

template <typename T, typename E>
bool DeviceLed<T, E>::getStatus()
{
    return status;
}

template <typename T, typename E>
void DeviceLed<T, E>::applyConfig(const DeviceLedConfig &config)
{
    // logger.log(LOG_INFO, [&]() -> String128
    //            {
    //             String128 buf;
    //             buf.add(F("(DeviceLed::applyConfig) Applying LED device configuration. CountLed: "));
    //             buf.add(config.countLed);
    //             buf.add(F(", Name: '"));
    //             buf.add(config.deviceName);
    //             buf.add(F("'"));
    //             return buf; });
    if (name != config.deviceName)
    {
        setName(config.deviceName);
    }

    if (countLed != config.countLed)
    {
        //TODO: надо как то метод котоырй будет говрит системе в ближайщее время сделай restart board
    }
}

template <typename T, typename E>
RgbColor DeviceLed<T, E>::stringHexToRgbColor(const char *colorStr)
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

template <typename T, typename E>
StringN<8> DeviceLed<T, E>::rgbColorToStringHex(const RgbColor &c)
{
    const char hexDigits[] = "0123456789abcdef";

    StringN<8> out;
    out += '#';

    out += hexDigits[c.R >> 4];
    out += hexDigits[c.R & 0x0F];

    out += hexDigits[c.G >> 4];
    out += hexDigits[c.G & 0x0F];

    out += hexDigits[c.B >> 4];
    out += hexDigits[c.B & 0x0F];

    return out;
}
