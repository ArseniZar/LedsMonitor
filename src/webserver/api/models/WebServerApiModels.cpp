#include "WebServerApiModels.h"

namespace api::webserver
{
    /*===================================== ScanWifiNetworkStarted ==========================================================*/

    namespace ScanWifiNetworkStartedResponceKey
    {
        constexpr const char *SCAN = "scan";
        constexpr const char *STARTED = "started";
    }

    ScanWifiNetworkStartedResponse::ScanWifiNetworkStartedResponse(const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    ScanWifiNetworkStartedResponse::ScanWifiNetworkStartedResponse(bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(status) {};

    gson::Str ScanWifiNetworkStartedResponse::toJson() const
    {
        using namespace ScanWifiNetworkStartedResponceKey;
        std::array<std::pair<const char *, Field>, 1> pairs = {{
            {STARTED, {status}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[SCAN] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== GetScanStatus ==========================================================*/

    namespace GetScanStatusResponseKey
    {
        constexpr const char *STATUS = "status";
        constexpr const char *CODE = "code";
    }

    GetScanStatusResponse::GetScanStatusResponse(const ScanState &status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    GetScanStatusResponse::GetScanStatusResponse(ScanState &&status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(std::move(status)) {};

    gson::Str GetScanStatusResponse::toJson() const
    {
        using namespace GetScanStatusResponseKey;
        std::array<std::pair<const char *, Field>, 1> pairs = {{
            {CODE, {static_cast<int>(status)}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== ScanNetworks ==========================================================*/

    namespace ScanNetworksResponceKey
    {
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
        constexpr const char *RSSI = "rssi";
        constexpr const char *ENCRYPTION = "encryptionType";
        constexpr const char *CHANNEL = "channel";
        constexpr const char *BSSID = "bssid";
        constexpr const char *HIDDEN = "hidden";
        constexpr const char *NETWORKS = "networks";
    }

    ScanWifiNetworkResponse::ScanWifiNetworkResponse(const std::vector<WifiNetwork> &networks, const ModelBaseResponse &base) : ModelBaseResponse(base), networks(networks) {};
    ScanWifiNetworkResponse::ScanWifiNetworkResponse(std::vector<WifiNetwork> &&networks, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), networks(std::move(networks)) {};

    gson::Str ScanWifiNetworkResponse::toJson() const
    {
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[ScanNetworksResponceKey::NETWORKS]('[');

        for (size_t i = 0; i < networks.size(); i++)
        {
            using namespace ScanNetworksResponceKey;
            std::array<std::pair<const char *, Field>, 7> pairs = {{
                {SSID, {networks[i].ssid}},
                {PASSWORD, {networks[i].password}},
                {RSSI, {networks[i].rssi}},
                {ENCRYPTION, {networks[i].encryptionType}},
                {CHANNEL, {networks[i].channel}},
                {BSSID, {networks[i].bssid}},
                {HIDDEN, {networks[i].hidden}}}};
            j += json::Serialization::serialization(pairs);
        }
        j(']');
        return j;
    }

    /*===================================== ConnectWifiNetwork ==========================================================*/

    namespace ConnectWifiNetworkRequestKey
    {
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
    }

    ConnectWifiNetworkRequest::ConnectWifiNetworkRequest(const char *ssid, const char *password, const ModelBaseRequest &base) : ModelBaseRequest(base), ssid(ssid), password(password) {}
    ConnectWifiNetworkRequest::ConnectWifiNetworkRequest(const char *ssid, const char *password, ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)), ssid(ssid), password(password) {}

    std::unique_ptr<json::Convertible> ConnectWifiNetworkRequest::fromJson(const gson::Entry &json)
    {
        using namespace ConnectWifiNetworkRequestKey;
        static const std::map<const char *, Field> keys = {
            {SSID, {json::ValueType::String32}},
            {PASSWORD, {json::ValueType::String32}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseConnectNetworkPtr = json::Parse::parse(json, keys);
        if (!parseConnectNetworkPtr->isOk())
        {
            return parseConnectNetworkPtr;
        }
        auto *successParseConnectNetworkPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value>> *>(parseConnectNetworkPtr.get());
        std::map<const char *, json::Value> parseMap = std::move(successParseConnectNetworkPtr->result);

        const String32 &ssid = std::get<String32>(parseMap[SSID]);
        const String32 &password = std::get<String32>(parseMap[PASSWORD]);

        return std::make_unique<json::ParseSuccess<ConnectWifiNetworkRequest>>(std::move(ConnectWifiNetworkRequest(ssid.c_str(), password.c_str(), std::move(successParseBasePtr->result))));
    }

    /*--------------------------------------------------------------------------------------------------------------*/

    namespace ConnectWifiNetworkStartedResponceKey
    {
        constexpr const char *CONNECT = "connect";
        constexpr const char *STARTED = "started";

    }

    ConnectWifiNetworkStartedResponse::ConnectWifiNetworkStartedResponse(const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {}
    ConnectWifiNetworkStartedResponse::ConnectWifiNetworkStartedResponse(bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(status) {}

    gson::Str ConnectWifiNetworkStartedResponse::toJson() const
    {
        using namespace ConnectWifiNetworkStartedResponceKey;
        std::array<std::pair<const char *, Field>, 1> pairs = {{
            {STARTED, {status}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONNECT] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== GetStatusWifi ==========================================================*/

    namespace GetStatusWifiResponceKey
    {
        constexpr const char *STATUS = "status";
        constexpr const char *CODE = "code";
    }

    GetWifiStatusResponse::GetWifiStatusResponse(const ConnState &status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    GetWifiStatusResponse::GetWifiStatusResponse(ConnState &&status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(std::move(status)) {};

    gson::Str GetWifiStatusResponse::toJson() const
    {
        using namespace GetStatusWifiResponceKey;
        std::array<std::pair<const char *, Field>, 1> pairs = {{
            {CODE, {static_cast<int>(status)}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== UpdateConfig ==========================================================*/

    namespace UpdateConfigRequestKey
    {
        constexpr const char *AP_SSID = "apSsid";
        constexpr const char *AP_PASSWORD = "apPassword";
        constexpr const char *MDNS_NAME = "mdnsName";
        constexpr const char *WIFI_CONNECTION_TIMEOUT = "wifiConnectionTimeout";
        constexpr const char *COUNT_LED = "countLed";
        constexpr const char *DEVICE_NAME = "deviceName";
        constexpr const char *TOKEN = "token";
        constexpr const char *LIMIT_MESSAGE = "limitMessage";
        constexpr const char *PERIOD_UPDATE = "periodUpdate";
    }

    UpdateConfigRequest::UpdateConfigRequest(const std::optional<String32> apSsid, const std::optional<String32> apPassword,
                                             const std::optional<String32> mdnsName, const std::optional<unsigned long> wifiConnectionTimeout,
                                             const std::optional<uint16_t> countLed, const std::optional<String32> deviceName,
                                             const std::optional<String64> token, const std::optional<uint8_t> limitMessage,
                                             const std::optional<uint16_t> periodUpdate, const ModelBaseRequest &base)
        : ModelBaseRequest(base), apSsid(apSsid),
          apPassword(apPassword), mdnsName(mdnsName),
          wifiConnectionTimeout(wifiConnectionTimeout), countLed(countLed),
          deviceName(deviceName), token(token),
          limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    UpdateConfigRequest::UpdateConfigRequest(std::optional<String32> &&apSsid, std::optional<String32> &&apPassword,
                                             std::optional<String32> &&mdnsName, std::optional<unsigned long> &&wifiConnectionTimeout,
                                             std::optional<uint16_t> &&countLed, std::optional<String32> &&deviceName,
                                             std::optional<String64> &&token, std::optional<uint8_t> &&limitMessage,
                                             std::optional<uint16_t> &&periodUpdate, ModelBaseRequest &&base)
        : ModelBaseRequest(std::move(base)), apSsid(std::move(apSsid)),
          apPassword(std::move(apPassword)), mdnsName(std::move(mdnsName)),
          wifiConnectionTimeout(std::move(wifiConnectionTimeout)), countLed(std::move(countLed)),
          deviceName(std::move(deviceName)), token(std::move(token)),
          limitMessage(std::move(limitMessage)), periodUpdate(std::move(periodUpdate)) {}

    std::unique_ptr<json::Convertible> UpdateConfigRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateConfigRequestKey;
        static const std::map<const char *, Field> keys = {
            {AP_SSID, {json::ValueType::String32, false}},
            {AP_PASSWORD, {json::ValueType::String32, false}},
            {MDNS_NAME, {json::ValueType::String32, false}},
            {WIFI_CONNECTION_TIMEOUT, {json::ValueType::U_Long, false}},
            {COUNT_LED, {json::ValueType::U_Int16, false}},
            {DEVICE_NAME, {json::ValueType::String32, false}},
            {TOKEN, {json::ValueType::String64, false}},
            {LIMIT_MESSAGE, {json::ValueType::U_Int8, false}},
            {PERIOD_UPDATE, {json::ValueType::U_Int16, false}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseConfigPtr = json::Parse::parse(json, keys);
        if (!parseConfigPtr->isOk())
        {
            return parseConfigPtr;
        }
        auto *successParseConfigPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value>> *>(parseConfigPtr.get());
        std::map<const char *, json::Value> parseMap = std::move(successParseConfigPtr->result);

        std::optional<String32> apSsid = json::Converter::toOptional<String32>(parseMap[AP_SSID]);
        std::optional<String32> apPassword = json::Converter::toOptional<String32>(parseMap[AP_PASSWORD]);
        std::optional<String32> mdnsName = json::Converter::toOptional<String32>(parseMap[MDNS_NAME]);
        std::optional<unsigned long> wifiConnectionTimeout = json::Converter::toOptional<unsigned long>(parseMap[WIFI_CONNECTION_TIMEOUT]);

        std::optional<String32> deviceName = json::Converter::toOptional<String32>(parseMap[DEVICE_NAME]);
        std::optional<String64> token = json::Converter::toOptional<String64>(parseMap[TOKEN]);
        std::optional<uint16_t> countLed = json::Converter::toOptional<uint16_t>(parseMap[COUNT_LED]);

        std::optional<uint8_t> limitMessage = json::Converter::toOptional<uint8_t>(parseMap[LIMIT_MESSAGE]);
        std::optional<uint16_t> periodUpdate = json::Converter::toOptional<uint16_t>(parseMap[PERIOD_UPDATE]);

        return std::make_unique<json::ParseSuccess<UpdateConfigRequest>>(
            UpdateConfigRequest(
                std::move(apSsid), std::move(apPassword),
                std::move(mdnsName), std::move(wifiConnectionTimeout),
                std::move(countLed), std::move(deviceName),
                std::move(token), std::move(limitMessage),
                std::move(periodUpdate), std::move(successParseBasePtr->result)));
    }

    /*===================================== GetConfig ==========================================================*/

    namespace GetConfigResponseKey
    {
        constexpr const char *CONFIG = "config";
        constexpr const char *AP_SSID = "apSsid";
        constexpr const char *AP_PASSWORD = "apPassword";
        constexpr const char *MDNS_NAME = "mdnsName";
        constexpr const char *WIFI_CONNECTION_TIMEOUT = "wifiConnectionTimeout";
        constexpr const char *COUNT_LED = "countLed";
        constexpr const char *DEVICE_NAME = "deviceName";
        constexpr const char *TOKEN = "token";
        constexpr const char *LIMIT_MESSAGE = "limitMessage";
        constexpr const char *PERIOD_UPDATE = "periodUpdate";
    }

    GetConfigResponse::GetConfigResponse(const String32 &apSsid, const String32 &apPassword,
                                         const String32 &mdnsName, const unsigned long wifiConnectionTimeout,
                                         const uint16_t countLed, const String32 &deviceName,
                                         const String64 &token, const uint8_t limitMessage,
                                         const uint16_t periodUpdate, const ModelBaseResponse &base)
        : ModelBaseResponse(base), apSsid(apSsid),
          apPassword(apPassword), mdnsName(mdnsName),
          wifiConnectionTimeout(wifiConnectionTimeout), countLed(countLed),
          deviceName(deviceName), token(token),
          limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    GetConfigResponse::GetConfigResponse(String32 &&apSsid, String32 &&apPassword,
                                         String32 &&mdnsName, unsigned long wifiConnectionTimeout,
                                         uint16_t countLed, String32 &&deviceName,
                                         String64 &&token, uint8_t limitMessage,
                                         uint16_t periodUpdate, ModelBaseResponse &&base)
        : ModelBaseResponse(std::move(base)), apSsid(std::move(apSsid)),
          apPassword(std::move(apPassword)), mdnsName(std::move(mdnsName)),
          wifiConnectionTimeout(wifiConnectionTimeout), countLed(countLed),
          deviceName(std::move(deviceName)), token(std::move(token)),
          limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    gson::Str GetConfigResponse::toJson() const
    {
        using namespace GetConfigResponseKey;
        std::array<std::pair<const char *, Field>, 9> pairs = {{
            {AP_SSID, {apSsid}},
            {AP_PASSWORD, {apPassword}},
            {MDNS_NAME, {mdnsName}},
            {WIFI_CONNECTION_TIMEOUT, {wifiConnectionTimeout}},
            {COUNT_LED, {countLed}},
            {DEVICE_NAME, {deviceName}},
            {TOKEN, {token}},
            {LIMIT_MESSAGE, {limitMessage}},
            {PERIOD_UPDATE, {periodUpdate}}}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONFIG] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== other ==========================================================*/
}
