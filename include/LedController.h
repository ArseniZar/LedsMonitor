#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "Adafruit_NeoPixel.h"
#include "config.h"
#include "Logger.h"

class LedController
{
private:
    Adafruit_NeoPixel device;
    Logger &logger;
    int brightness;
    bool status;
    uint32_t color;

    uint32_t hexToColor(uint32_t hex);
    uint32_t stringToHex(String color);

public:
    LedController(Logger &logger);
    void begin();
    void onDevice();
    void offDevice();
    bool setBrightness(int new_brightness);
    void setColor(String color);
    bool getStatus();
};

#endif