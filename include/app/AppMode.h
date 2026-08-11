#pragma once
#ifndef APP_MODE_H
#define APP_MODE_H

#include <Arduino.h>

enum class AppMode : uint8_t
{
    CONFIG,
    WORK
};

#endif // APP_MODE