#pragma once
#ifndef WIFI_STATUS_H
#define WIFI_STATUS_H
#include <Arduino.h>

enum class ConnState : uint8_t
{
    WL_NO_SHIELD = 255,
    WL_IDLE_STATUS = 0,
    WL_NO_SSID_AVAIL = 1,
    WL_SCAN_COMPLETED = 2,
    WL_CONNECTED = 3,
    WL_CONNECT_FAILED = 4,
    WL_CONNECTION_LOST = 5,
    WL_WRONG_PASSWORD = 6,
    WL_DISCONNECTED = 7

};

enum class ScanState : int8_t
{
    FAILED = -2,
    RUNNING = -1,
    COMPLETED = 0
};

#endif // WIFI_STATUS_H