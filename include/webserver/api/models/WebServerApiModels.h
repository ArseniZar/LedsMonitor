#pragma once
#ifndef WEBSERVER_API_MODELS_H
#define WEBSERVER_API_MODELS_H

#include <Arduino.h>
#include <optional>
#include "WebServerApiModelsBase.h"
#include "WifiNetwork.h"
#include "WifiStatus.h"

namespace api::webserver
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

    /*===================================== UpdateNetworkConfig =======================================================*/
    class UpdateNetworkConfigRequest final : public ModelBaseRequest
    {
    public:
        std::optional<String32> apSsid;
        std::optional<String32> apPassword;
        std::optional<String32> mdnsName;
        std::optional<uint32_t> wifiConnectionTimeoutMs;
        UpdateNetworkConfigRequest() = delete;
        UpdateNetworkConfigRequest(const std::optional<String32> apSsid, const std::optional<String32> apPassword,
                                   const std::optional<String32> mdnsName, const std::optional<uint32_t> wifiConnectionTimeoutMs, const ModelBaseRequest &base);
        UpdateNetworkConfigRequest(std::optional<String32> &&apSsid, std::optional<String32> &&apPassword,
                                   std::optional<String32> &&mdnsName, std::optional<uint32_t> &&wifiConnectionTimeoutMs, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== UpdateDeviceLedConfig =======================================================*/
    class UpdateDeviceLedConfigRequest final : public ModelBaseRequest
    {
    public:
        std::optional<uint16_t> countLed;
        std::optional<String32> deviceName;
        UpdateDeviceLedConfigRequest() = delete;
        UpdateDeviceLedConfigRequest(const std::optional<uint16_t> countLed, const std::optional<String32> deviceName, const ModelBaseRequest &base);
        UpdateDeviceLedConfigRequest(std::optional<uint16_t> &&countLed, std::optional<String32> &&deviceName, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== UpdateTelegramBotConfig =======================================================*/
    class UpdateTelegramBotConfigRequest final : public ModelBaseRequest
    {
    public:
        std::optional<String64> token;
        std::optional<uint8_t> limitMessage;
        std::optional<uint16_t> periodUpdate;
        UpdateTelegramBotConfigRequest() = delete;
        UpdateTelegramBotConfigRequest(const std::optional<String64> token, const std::optional<uint8_t> limitMessage, const std::optional<uint16_t> periodUpdate, const ModelBaseRequest &base);
        UpdateTelegramBotConfigRequest(std::optional<String64> &&token, std::optional<uint8_t> &&limitMessage, std::optional<uint16_t> &&periodUpdate, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== UpdateAppConfig =======================================================*/
    class UpdateAppConfigRequest final : public ModelBaseRequest
    {
    public:
        std::optional<uint32_t> webServerInactivityPeriodMs;
        std::optional<uint32_t> wifiReconnectPeriodMs;
        std::optional<uint32_t> applyConfigTimeoutMs;
        std::optional<uint32_t> saveConfigTimeoutMs;
        std::optional<uint32_t> holdButtonTimeoutMs;
        UpdateAppConfigRequest() = delete;
        UpdateAppConfigRequest(const std::optional<uint32_t> webServerInactivityPeriodMs, const std::optional<uint32_t> wifiReconnectPeriodMs,
                               const std::optional<uint32_t> applyConfigTimeoutMs, const std::optional<uint32_t> saveConfigTimeoutMs, const std::optional<uint32_t> holdButtonTimeoutMs, const ModelBaseRequest &base);
        UpdateAppConfigRequest(std::optional<uint32_t> &&webServerInactivityPeriodMs, std::optional<uint32_t> &&wifiReconnectPeriodMs,
                               std::optional<uint32_t> &&applyConfigTimeoutMs, std::optional<uint32_t> &&saveConfigTimeoutMs, std::optional<uint32_t> &&holdButtonTimeoutMs, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== GetNetworkConfig =======================================================*/
    class GetNetworkConfigResponse final : public ModelBaseResponse
    {
    public:
        String32 ssid;
        String32 password;
        String32 apSsid;
        String32 apPassword;
        String32 mdnsName;
        uint32_t wifiConnectionTimeoutMs;
        GetNetworkConfigResponse() = delete;
        GetNetworkConfigResponse(const String32 &ssid, const String32 &password, const String32 &apSsid, const String32 &apPassword,
                                 const String32 &mdnsName, const uint32_t wifiConnectionTimeoutMs, const ModelBaseResponse &base);
        GetNetworkConfigResponse(String32 &&ssid, String32 &&password, String32 &&apSsid, String32 &&apPassword,
                                 String32 &&mdnsName, uint32_t wifiConnectionTimeoutMs, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetDeviceLedConfig =======================================================*/
    class GetDeviceLedConfigResponse final : public ModelBaseResponse
    {
    public:
        uint8_t pin;
        uint16_t countLed;
        String32 deviceName;
        GetDeviceLedConfigResponse() = delete;
        GetDeviceLedConfigResponse(const uint8_t pin, const uint16_t countLed, const String32 &deviceName, const ModelBaseResponse &base);
        GetDeviceLedConfigResponse(uint8_t pin, uint16_t countLed, String32 &&deviceName, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetTelegramBotConfig =======================================================*/
    class GetTelegramBotConfigResponse final : public ModelBaseResponse
    {
    public:
        String64 token;
        uint8_t limitMessage;
        uint16_t periodUpdate;
        GetTelegramBotConfigResponse() = delete;
        GetTelegramBotConfigResponse(const String64 &token, const uint8_t limitMessage, const uint16_t periodUpdate, const ModelBaseResponse &base);
        GetTelegramBotConfigResponse(String64 &&token, uint8_t limitMessage, uint16_t periodUpdate, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetAppConfig =======================================================*/
    class GetAppConfigResponse final : public ModelBaseResponse
    {
    public:
        uint32_t webServerInactivityPeriodMs;
        uint32_t wifiReconnectPeriodMs;
        uint32_t applyConfigTimeoutMs;
        uint32_t saveConfigTimeoutMs;
        uint32_t holdButtonTimeoutMs;
        uint8_t buttonPin;
        GetAppConfigResponse() = delete;
        GetAppConfigResponse(const uint32_t webServerInactivityPeriodMs, const uint32_t wifiReconnectPeriodMs,
                             const uint32_t applyConfigTimeoutMs, const uint32_t saveConfigTimeoutMs, const uint32_t holdButtonTimeoutMs, const uint8_t buttonPin, const ModelBaseResponse &base);
        GetAppConfigResponse(uint32_t webServerInactivityPeriodMs, uint32_t wifiReconnectPeriodMs,
                             uint32_t applyConfigTimeoutMs, uint32_t saveConfigTimeoutMs, uint32_t holdButtonTimeoutMs, uint8_t buttonPin, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== other =======================================================*/
}
#endif // WEBSERVER_API_MODELS_H
