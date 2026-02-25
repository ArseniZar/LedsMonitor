#pragma once
#ifndef WIFI_TYPES_H
#define WIFI_TYPES_H

enum class ConnState : int
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

enum class ScanState : int 
{
    FAILED = -2,        // Scan failed (WIFI_SCAN_FAILED)
    RUNNING = -1,       // Scan in progress (WIFI_SCAN_RUNNING)
    COMPLETED = 0       // Scan completed (0 or more networks found)
};

// enum class WifiFailState : int
// {
//     WIFI_DISCONNECT_REASON_BEACON_TIMEOUT     = 200,
//     WIFI_DISCONNECT_REASON_NO_AP_FOUND        = 201,
//     WIFI_DISCONNECT_REASON_AUTH_FAIL          = 202,
//     WIFI_DISCONNECT_REASON_ASSOC_FAIL         = 203,
//     WIFI_DISCONNECT_REASON_HANDSHAKE_TIMEOUT  = 204,
// };

#endif