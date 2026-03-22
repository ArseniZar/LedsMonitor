#pragma once
#ifndef DEVICE_MODELS_H
#define DEVICE_MODELS_H

#include <Arduino.h>
#include <optional>
#include "ApiModelsBase.h"
#include "WifiNetwork.h"
#include "WifiStates.h" 
// TODO: назвать файл по другому ApiDeviceModels.h
namespace api
{
    class ScanWifiNetworkStartedResponse final : public ModelBaseResponse
    {
    public:
        bool status;
        ScanWifiNetworkStartedResponse() = delete;
        ScanWifiNetworkStartedResponse(const bool status, const ModelBaseResponse &base);
        ScanWifiNetworkStartedResponse(bool status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetScanStatus =======================================================*/

    class GetScanStatusResponse final : public ModelBaseResponse
    {
    public:
        ScanState status;
        GetScanStatusResponse() = delete;
        GetScanStatusResponse(const ScanState &status, const ModelBaseResponse &base);
        GetScanStatusResponse(ScanState &&status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== ScanWifiNetwork =======================================================*/

    class ScanWifiNetworkResponse final : public ModelBaseResponse
    {
    public:
        std::vector<WifiNetwork> networks;
        ScanWifiNetworkResponse() = delete;
        ScanWifiNetworkResponse(const std::vector<WifiNetwork> &networks, const ModelBaseResponse &base);
        ScanWifiNetworkResponse(std::vector<WifiNetwork> &&networks, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== ConnectWifiNetworkStarted ====================================================*/

    class ConnectWifiNetworkRequest final : public ModelBaseRequest
    {
    public:
        String32 ssid;
        String32 password;
        ConnectWifiNetworkRequest() = delete;
        ConnectWifiNetworkRequest(const char *ssid, const char *password, const ModelBaseRequest &base);
        ConnectWifiNetworkRequest(const char *ssid, const char *password, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    class ConnectWifiNetworkStartedResponse final : public ModelBaseResponse
    {
    public:
        bool status;
        ConnectWifiNetworkStartedResponse() = delete;
        ConnectWifiNetworkStartedResponse(const bool status, const ModelBaseResponse &base);
        ConnectWifiNetworkStartedResponse(bool status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetStatusWifi =======================================================*/

    class GetWifiStatusResponse final : public ModelBaseResponse
    {
    public:
        ConnState status;
        GetWifiStatusResponse() = delete;
        GetWifiStatusResponse(const ConnState &status, const ModelBaseResponse &base);
        GetWifiStatusResponse(ConnState &&status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== UpdateConfig =======================================================*/

    class UpdateConfigRequest final : public ModelBaseRequest
    {
    public:
        std::optional<String32> apSsid;
        std::optional<String32> apPassword;
        std::optional<String32> mdnsName;
        std::optional<unsigned long> wifiConnectionTimeout;

        std::optional<uint16_t> countLed;
        std::optional<String32> deviceName;

        std::optional<String64> token;
        std::optional<uint8_t> limitMessage;
        std::optional<uint16_t> periodUpdate;
        UpdateConfigRequest() = delete;
        UpdateConfigRequest(const std::optional<String32> apSsid, const std::optional<String32> apPassword,
                            const std::optional<String32> mdnsName, const std::optional<unsigned long> wifiConnectionTimeout,
                            const std::optional<uint16_t> countLed, const std::optional<String32> deviceName,
                            const std::optional<String64> token, const std::optional<uint8_t> limitMessage,
                            const std::optional<uint16_t> periodUpdate, const ModelBaseRequest &base);
        UpdateConfigRequest(std::optional<String32> &&apSsid, std::optional<String32> &&apPassword,
                            std::optional<String32> &&mdnsName, std::optional<unsigned long> &&wifiConnectionTimeout,
                            std::optional<uint16_t> &&countLed, std::optional<String32> &&deviceName,
                            std::optional<String64> &&token, std::optional<uint8_t> &&limitMessage,
                            std::optional<uint16_t> &&periodUpdate, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== GetConfig =======================================================*/

    class GetConfigResponse final : public ModelBaseResponse
    {
    public:
        String32 apSsid;
        String32 apPassword;
        String32 mdnsName;
        unsigned long wifiConnectionTimeout;

        uint16_t countLed;
        String32 deviceName;

        String64 token;
        uint8_t limitMessage;
        uint16_t periodUpdate;
        GetConfigResponse() = delete;
        GetConfigResponse(const String32 &apSsid, const String32 &apPassword,
                          const String32 &mdnsName, const unsigned long wifiConnectionTimeout,
                          const uint16_t countLed, const String32 &deviceName,
                          const String64 &token, const uint8_t limitMessage,
                          const uint16_t periodUpdate, const ModelBaseResponse &base);
        GetConfigResponse(String32 &&apSsid, String32 &&apPassword,
                          String32 &&mdnsName, unsigned long wifiConnectionTimeout,
                          uint16_t countLed, String32 &&deviceName,
                          String64 &&token, uint8_t limitMessage,
                          uint16_t periodUpdate, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== other =======================================================*/
}
#endif // DEVICE_MODELS_H
