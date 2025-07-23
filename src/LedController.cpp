#include "LedController.h"

LedController::LedController(Logger &logger) : device(LED_COUNT, DEVICE_PIN, NEO_BRG + NEO_KHZ800), logger(logger)
{
}

void LedController::begin()
{
    device.begin();
    setBrightness(DEVICE_BRIGHTNESS);
}

bool LedController::setBrightness(int new_brightness)
{
    if (new_brightness <= 255 && new_brightness >= 0)
    {
        brightness = new_brightness;
        device.setBrightness(brightness);
        device.show();
        return true;
    }
    return false;
}

void LedController::onDevice()
{
    status = false;
    for (int i = 0; i < LED_COUNT; i++)
    {
        device.setPixelColor(i, device.Color(255, 255, 255));
    }
    device.show();
}
void LedController::offDevice()
{
    status = false;
    for (int i = 0; i < LED_COUNT; i++)
    {
        device.setPixelColor(i, device.Color(0, 0, 0));
    }
    device.show();
}

void LedController::setColor(String color)
{
    uint32_t colorHex = hexToColor(stringToHex(color));
    color = colorHex;
    for (int i = 0; i < LED_COUNT; i++)
    {
        device.setPixelColor(i, colorHex);
    }
    device.show();
}

uint32_t LedController::hexToColor(uint32_t hex)
{
    uint8_t r = (hex >> 16) & 0xFF;
    uint8_t g = (hex >> 8) & 0xFF;
    uint8_t b = hex & 0xFF;

    return device.Color(r, g, b);
}

uint32_t LedController::stringToHex(String color)
{
    if (color.startsWith("#"))
    {
        color = color.substring(1);
    }

    char buf[7];
    color.toCharArray(buf, 7);

    return strtoul(buf, NULL, 16);
}

bool LedController::getStatus()
{
    return status;
}