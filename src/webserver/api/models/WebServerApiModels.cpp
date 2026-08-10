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
        std::array<std::pair<const char *, Field>, 1> pairs = {{{STARTED, {status}}}};
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
        std::array<std::pair<const char *, Field>, 1> pairs = {{{CODE, {static_cast<int>(status)}}}};
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
            std::array<std::pair<const char *, Field>, 7> pairs = {{{SSID, {networks[i].ssid}},
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
        static const std::map<const char *, Field, StrCompare> keys = {
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
        auto *successParseConnectNetworkPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseConnectNetworkPtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseConnectNetworkPtr->result);

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
        std::array<std::pair<const char *, Field>, 1> pairs = {{{STARTED, {status}}}};
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
        std::array<std::pair<const char *, Field>, 1> pairs = {{{CODE, {static_cast<int>(status)}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== UpdateNetworkConfig ==========================================================*/
    namespace UpdateNetworkConfigRequestKey
    {
        constexpr const char *AP_SSID = "apSsid";
        constexpr const char *AP_PASSWORD = "apPassword";
        constexpr const char *MDNS_NAME = "mdnsName";
        constexpr const char *WIFI_CONNECTION_TIMEOUT_MS = "wifiConnectionTimeoutMs";
    }

    UpdateNetworkConfigRequest::UpdateNetworkConfigRequest(const std::optional<String32> apSsid, const std::optional<String32> apPassword,
                                                           const std::optional<String32> mdnsName, const std::optional<uint32_t> wifiConnectionTimeoutMs,
                                                           const ModelBaseRequest &base)
        : ModelBaseRequest(base), apSsid(apSsid),
          apPassword(apPassword), mdnsName(mdnsName),
          wifiConnectionTimeoutMs(wifiConnectionTimeoutMs) {}

    UpdateNetworkConfigRequest::UpdateNetworkConfigRequest(std::optional<String32> &&apSsid, std::optional<String32> &&apPassword,
                                                           std::optional<String32> &&mdnsName, std::optional<uint32_t> &&wifiConnectionTimeoutMs,
                                                           ModelBaseRequest &&base)
        : ModelBaseRequest(std::move(base)), apSsid(std::move(apSsid)),
          apPassword(std::move(apPassword)), mdnsName(std::move(mdnsName)),
          wifiConnectionTimeoutMs(std::move(wifiConnectionTimeoutMs)) {}

    std::unique_ptr<json::Convertible> UpdateNetworkConfigRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateNetworkConfigRequestKey;
        static const std::map<const char *, Field, StrCompare> keys = {
            {AP_SSID, {json::ValueType::String32, false}},
            {AP_PASSWORD, {json::ValueType::String32, false}},
            {MDNS_NAME, {json::ValueType::String32, false}},
            {WIFI_CONNECTION_TIMEOUT_MS, {json::ValueType::U_Int32, false}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseNetworkConfigPtr = json::Parse::parse(json, keys);
        if (!parseNetworkConfigPtr->isOk())
        {
            return parseNetworkConfigPtr;
        }
        auto *successParseNetworkConfigPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseNetworkConfigPtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseNetworkConfigPtr->result);

        std::optional<String32> apSsid = json::Converter::toOptional<String32>(parseMap[AP_SSID]);
        std::optional<String32> apPassword = json::Converter::toOptional<String32>(parseMap[AP_PASSWORD]);
        std::optional<String32> mdnsName = json::Converter::toOptional<String32>(parseMap[MDNS_NAME]);
        std::optional<uint32_t> wifiConnectionTimeoutMs = json::Converter::toOptional<uint32_t>(parseMap[WIFI_CONNECTION_TIMEOUT_MS]);

        return std::make_unique<json::ParseSuccess<UpdateNetworkConfigRequest>>(
            UpdateNetworkConfigRequest(
                std::move(apSsid), std::move(apPassword),
                std::move(mdnsName), std::move(wifiConnectionTimeoutMs),
                std::move(successParseBasePtr->result)));
    }

    /*===================================== UpdateDeviceLedConfig ==========================================================*/
    namespace UpdateDeviceLedConfigRequestKey
    {
        constexpr const char *COUNT_LED = "countLed";
        constexpr const char *DEVICE_NAME = "deviceName";
    }

    UpdateDeviceLedConfigRequest::UpdateDeviceLedConfigRequest(const std::optional<uint16_t> countLed, const std::optional<String32> deviceName, const ModelBaseRequest &base)
        : ModelBaseRequest(base), countLed(countLed), deviceName(deviceName) {}

    UpdateDeviceLedConfigRequest::UpdateDeviceLedConfigRequest(std::optional<uint16_t> &&countLed, std::optional<String32> &&deviceName, ModelBaseRequest &&base)
        : ModelBaseRequest(std::move(base)), countLed(std::move(countLed)), deviceName(std::move(deviceName)) {}

    std::unique_ptr<json::Convertible> UpdateDeviceLedConfigRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateDeviceLedConfigRequestKey;
        static const std::map<const char *, Field, StrCompare> keys = {
            {COUNT_LED, {json::ValueType::U_Int16, false}},
            {DEVICE_NAME, {json::ValueType::String32, false}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseDeviceLedConfigPtr = json::Parse::parse(json, keys);
        if (!parseDeviceLedConfigPtr->isOk())
        {
            return parseDeviceLedConfigPtr;
        }
        auto *successParseDeviceLedConfigPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseDeviceLedConfigPtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseDeviceLedConfigPtr->result);

        std::optional<uint16_t> countLed = json::Converter::toOptional<uint16_t>(parseMap[COUNT_LED]);
        std::optional<String32> deviceName = json::Converter::toOptional<String32>(parseMap[DEVICE_NAME]);

        return std::make_unique<json::ParseSuccess<UpdateDeviceLedConfigRequest>>(
            UpdateDeviceLedConfigRequest(
                std::move(countLed), std::move(deviceName),
                std::move(successParseBasePtr->result)));
    }

    /*===================================== UpdateTelegramBotConfig ==========================================================*/
    namespace UpdateTelegramBotConfigRequestKey
    {
        constexpr const char *TOKEN = "token";
        constexpr const char *LIMIT_MESSAGE = "limitMessage";
        constexpr const char *PERIOD_UPDATE = "periodUpdate";
    }

    UpdateTelegramBotConfigRequest::UpdateTelegramBotConfigRequest(const std::optional<String64> token, const std::optional<uint8_t> limitMessage, const std::optional<uint16_t> periodUpdate, const ModelBaseRequest &base)
        : ModelBaseRequest(base), token(token), limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    UpdateTelegramBotConfigRequest::UpdateTelegramBotConfigRequest(std::optional<String64> &&token, std::optional<uint8_t> &&limitMessage, std::optional<uint16_t> &&periodUpdate, ModelBaseRequest &&base)
        : ModelBaseRequest(std::move(base)), token(std::move(token)), limitMessage(std::move(limitMessage)), periodUpdate(std::move(periodUpdate)) {}

    std::unique_ptr<json::Convertible> UpdateTelegramBotConfigRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateTelegramBotConfigRequestKey;
        static const std::map<const char *, Field, StrCompare> keys = {
            {TOKEN, {json::ValueType::String64, false}},
            {LIMIT_MESSAGE, {json::ValueType::U_Int8, false}},
            {PERIOD_UPDATE, {json::ValueType::U_Int16, false}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseTelegramBotConfigPtr = json::Parse::parse(json, keys);
        if (!parseTelegramBotConfigPtr->isOk())
        {
            return parseTelegramBotConfigPtr;
        }
        auto *successParseTelegramBotConfigPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseTelegramBotConfigPtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseTelegramBotConfigPtr->result);

        std::optional<String64> token = json::Converter::toOptional<String64>(parseMap[TOKEN]);
        std::optional<uint8_t> limitMessage = json::Converter::toOptional<uint8_t>(parseMap[LIMIT_MESSAGE]);
        std::optional<uint16_t> periodUpdate = json::Converter::toOptional<uint16_t>(parseMap[PERIOD_UPDATE]);

        return std::make_unique<json::ParseSuccess<UpdateTelegramBotConfigRequest>>(
            UpdateTelegramBotConfigRequest(
                std::move(token), std::move(limitMessage), std::move(periodUpdate),
                std::move(successParseBasePtr->result)));
    }

    /*===================================== UpdateAppConfig ==========================================================*/
    namespace UpdateAppConfigRequestKey
    {
        constexpr const char *WEBSERVER_INACTIVITY_PERIOD_MS = "webServerInactivityPeriodMs";
        constexpr const char *WIFI_RECONNECT_PERIOD_MS = "wifiReconnectPeriodMs";
        constexpr const char *APPLY_CONFIG_TIMEOUT_MS = "applyConfigTimeoutMs";
        constexpr const char *SAVE_CONFIG_TIMEOUT_MS = "saveConfigTimeoutMs";
        constexpr const char *HOLD_BUTTON_TIMEOUT_MS = "holdButtonTimeoutMs";
    }

    UpdateAppConfigRequest::UpdateAppConfigRequest(const std::optional<uint32_t> webServerInactivityPeriodMs, const std::optional<uint32_t> wifiReconnectPeriodMs,
                                                   const std::optional<uint32_t> applyConfigTimeoutMs, const std::optional<uint32_t> saveConfigTimeoutMs, const std::optional<uint32_t> holdButtonTimeoutMs,
                                                   const ModelBaseRequest &base)
        : ModelBaseRequest(base), webServerInactivityPeriodMs(webServerInactivityPeriodMs), wifiReconnectPeriodMs(wifiReconnectPeriodMs),
          applyConfigTimeoutMs(applyConfigTimeoutMs), saveConfigTimeoutMs(saveConfigTimeoutMs), holdButtonTimeoutMs(holdButtonTimeoutMs) {}

    UpdateAppConfigRequest::UpdateAppConfigRequest(std::optional<uint32_t> &&webServerInactivityPeriodMs, std::optional<uint32_t> &&wifiReconnectPeriodMs,
                                                   std::optional<uint32_t> &&applyConfigTimeoutMs, std::optional<uint32_t> &&saveConfigTimeoutMs, std::optional<uint32_t> &&holdButtonTimeoutMs,
                                                   ModelBaseRequest &&base)
        : ModelBaseRequest(std::move(base)), webServerInactivityPeriodMs(std::move(webServerInactivityPeriodMs)), wifiReconnectPeriodMs(std::move(wifiReconnectPeriodMs)),
          applyConfigTimeoutMs(std::move(applyConfigTimeoutMs)), saveConfigTimeoutMs(std::move(saveConfigTimeoutMs)), holdButtonTimeoutMs(std::move(holdButtonTimeoutMs)) {}    

    std::unique_ptr<json::Convertible> UpdateAppConfigRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateAppConfigRequestKey;
        static const std::map<const char *, Field, StrCompare> keys = {
            {WEBSERVER_INACTIVITY_PERIOD_MS, {json::ValueType::U_Int32, false}},
            {WIFI_RECONNECT_PERIOD_MS, {json::ValueType::U_Int32, false}},
            {APPLY_CONFIG_TIMEOUT_MS, {json::ValueType::U_Int32, false}},
            {SAVE_CONFIG_TIMEOUT_MS, {json::ValueType::U_Int32, false}},
            {HOLD_BUTTON_TIMEOUT_MS, {json::ValueType::U_Int32, false}}};
        
        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());    

        auto parseAppConfigPtr = json::Parse::parse(json, keys);
        if (!parseAppConfigPtr->isOk())
        {
            return parseAppConfigPtr;
        }
        auto *successParseAppConfigPtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseAppConfigPtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseAppConfigPtr->result);

        std::optional<uint32_t> webServerInactivityPeriodMs = json::Converter::toOptional<uint32_t>(parseMap[WEBSERVER_INACTIVITY_PERIOD_MS]);
        std::optional<uint32_t> wifiReconnectPeriodMs = json::Converter::toOptional<uint32_t>(parseMap[WIFI_RECONNECT_PERIOD_MS]);
        std::optional<uint32_t> applyConfigTimeoutMs = json::Converter::toOptional<uint32_t>(parseMap[APPLY_CONFIG_TIMEOUT_MS]);
        std::optional<uint32_t> saveConfigTimeoutMs = json::Converter::toOptional<uint32_t>(parseMap[SAVE_CONFIG_TIMEOUT_MS]);
        std::optional<uint32_t> holdButtonTimeoutMs = json::Converter::toOptional<uint32_t>(parseMap[HOLD_BUTTON_TIMEOUT_MS]);

        return std::make_unique<json::ParseSuccess<UpdateAppConfigRequest>>(
            UpdateAppConfigRequest(
                std::move(webServerInactivityPeriodMs), std::move(wifiReconnectPeriodMs),
                std::move(applyConfigTimeoutMs), std::move(saveConfigTimeoutMs), std::move(holdButtonTimeoutMs),
                std::move(successParseBasePtr->result)));
    }

    /*===================================== GetNetworkConfig ==========================================================*/
    namespace GetNetworkConfigResponseKey
    {
        constexpr const char *CONFIG = "config";
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
        constexpr const char *AP_SSID = "apSsid";
        constexpr const char *AP_PASSWORD = "apPassword";
        constexpr const char *MDNS_NAME = "mdnsName";
        constexpr const char *WIFI_CONNECTION_TIMEOUT_MS = "wifiConnectionTimeoutMs";
    }

    GetNetworkConfigResponse::GetNetworkConfigResponse(const String32 &ssid, const String32 &password,
                                                       const String32 &apSsid, const String32 &apPassword,
                                                       const String32 &mdnsName, const uint32_t wifiConnectionTimeoutMs,
                                                       const ModelBaseResponse &base)
        : ModelBaseResponse(base), ssid(ssid), password(password),
          apSsid(apSsid), apPassword(apPassword),
          mdnsName(mdnsName), wifiConnectionTimeoutMs(wifiConnectionTimeoutMs) {}

    GetNetworkConfigResponse::GetNetworkConfigResponse(String32 &&ssid, String32 &&password,
                                                       String32 &&apSsid, String32 &&apPassword,
                                                       String32 &&mdnsName, uint32_t wifiConnectionTimeoutMs,
                                                       ModelBaseResponse &&base)
        : ModelBaseResponse(std::move(base)), ssid(std::move(ssid)), password(std::move(password)),
          apSsid(std::move(apSsid)), apPassword(std::move(apPassword)),
          mdnsName(std::move(mdnsName)), wifiConnectionTimeoutMs(wifiConnectionTimeoutMs) {}

    gson::Str GetNetworkConfigResponse::toJson() const
    {
        using namespace GetNetworkConfigResponseKey;
        std::array<std::pair<const char *, Field>, 6> pairs = {{{SSID, {ssid}},
                                                                {PASSWORD, {password}},
                                                                {AP_SSID, {apSsid}},
                                                                {AP_PASSWORD, {apPassword}},
                                                                {MDNS_NAME, {mdnsName}},
                                                                {WIFI_CONNECTION_TIMEOUT_MS, {wifiConnectionTimeoutMs}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONFIG] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== GetDeviceLedConfig ==========================================================*/
    namespace GetDeviceLedConfigResponseKey
    {
        constexpr const char *CONFIG = "config";
        constexpr const char *PIN = "pin";
        constexpr const char *COUNT_LED = "countLed";
        constexpr const char *DEVICE_NAME = "deviceName";
    }

    GetDeviceLedConfigResponse::GetDeviceLedConfigResponse(const uint8_t pin, const uint16_t countLed, const String32 &deviceName, const ModelBaseResponse &base)
        : ModelBaseResponse(base), pin(pin), countLed(countLed), deviceName(deviceName) {}

    GetDeviceLedConfigResponse::GetDeviceLedConfigResponse(uint8_t pin, uint16_t countLed, String32 &&deviceName, ModelBaseResponse &&base)
        : ModelBaseResponse(std::move(base)), pin(pin), countLed(countLed), deviceName(std::move(deviceName)) {}

    gson::Str GetDeviceLedConfigResponse::toJson() const
    {
        using namespace GetDeviceLedConfigResponseKey;
        std::array<std::pair<const char *, Field>, 3> pairs = {{{PIN, {pin}},
                                                                {COUNT_LED, {countLed}},
                                                                {DEVICE_NAME, {deviceName}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONFIG] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== GetTelegramBotConfig ==========================================================*/
    namespace GetTelegramBotConfigResponseKey
    {
        constexpr const char *CONFIG = "config";
        constexpr const char *TOKEN = "token";
        constexpr const char *LIMIT_MESSAGE = "limitMessage";
        constexpr const char *PERIOD_UPDATE = "periodUpdate";
    }

    GetTelegramBotConfigResponse::GetTelegramBotConfigResponse(const String64 &token, const uint8_t limitMessage, const uint16_t periodUpdate, const ModelBaseResponse &base)
        : ModelBaseResponse(base), token(token), limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    GetTelegramBotConfigResponse::GetTelegramBotConfigResponse(String64 &&token, uint8_t limitMessage, uint16_t periodUpdate, ModelBaseResponse &&base)
        : ModelBaseResponse(std::move(base)), token(std::move(token)), limitMessage(limitMessage), periodUpdate(periodUpdate) {}

    gson::Str GetTelegramBotConfigResponse::toJson() const
    {
        using namespace GetTelegramBotConfigResponseKey;
        std::array<std::pair<const char *, Field>, 3> pairs = {{{TOKEN, {token}},
                                                                {LIMIT_MESSAGE, {limitMessage}},
                                                                {PERIOD_UPDATE, {periodUpdate}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONFIG] = json::Serialization::serialization(pairs);
        return j;
    }
    /*===================================== GetAppConfig ==========================================================*/
    namespace GetAppConfigResponseKey
    {
        constexpr const char *CONFIG = "config";
        constexpr const char *WEBSERVER_INACTIVITY_PERIOD_MS = "webServerInactivityPeriodMs";
        constexpr const char *WIFI_RECONNECT_PERIOD_MS = "wifiReconnectPeriodMs";
        constexpr const char *APPLY_CONFIG_TIMEOUT_MS = "applyConfigTimeoutMs";
        constexpr const char *SAVE_CONFIG_TIMEOUT_MS = "saveConfigTimeoutMs";
        constexpr const char *HOLD_BUTTON_TIMEOUT_MS = "holdButtonTimeoutMs";
        constexpr const char *BUTTON_PIN = "buttonPin";
    }

    GetAppConfigResponse::GetAppConfigResponse(const uint32_t webServerInactivityPeriodMs, const uint32_t wifiReconnectPeriodMs,
                                               const uint32_t applyConfigTimeoutMs, const uint32_t saveConfigTimeoutMs,
                                               const uint32_t holdButtonTimeoutMs, const uint8_t buttonPin,
                                               const ModelBaseResponse &base)
        : ModelBaseResponse(base), webServerInactivityPeriodMs(webServerInactivityPeriodMs),
          wifiReconnectPeriodMs(wifiReconnectPeriodMs), applyConfigTimeoutMs(applyConfigTimeoutMs),
          saveConfigTimeoutMs(saveConfigTimeoutMs), holdButtonTimeoutMs(holdButtonTimeoutMs),
          buttonPin(buttonPin) {}

    GetAppConfigResponse::GetAppConfigResponse(uint32_t webServerInactivityPeriodMs, uint32_t wifiReconnectPeriodMs,
                                               uint32_t applyConfigTimeoutMs, uint32_t saveConfigTimeoutMs,
                                               uint32_t holdButtonTimeoutMs, uint8_t buttonPin,
                                               ModelBaseResponse &&base)
        : ModelBaseResponse(std::move(base)), webServerInactivityPeriodMs(webServerInactivityPeriodMs),
          wifiReconnectPeriodMs(wifiReconnectPeriodMs), applyConfigTimeoutMs(applyConfigTimeoutMs),
          saveConfigTimeoutMs(saveConfigTimeoutMs), holdButtonTimeoutMs(holdButtonTimeoutMs),
          buttonPin(buttonPin) {}

    gson::Str GetAppConfigResponse::toJson() const
    {
        using namespace GetAppConfigResponseKey;
        std::array<std::pair<const char *, Field>, 6> pairs = {{{WEBSERVER_INACTIVITY_PERIOD_MS, {webServerInactivityPeriodMs}},
                                                                {WIFI_RECONNECT_PERIOD_MS, {wifiReconnectPeriodMs}},
                                                                {APPLY_CONFIG_TIMEOUT_MS, {applyConfigTimeoutMs}},
                                                                {SAVE_CONFIG_TIMEOUT_MS, {saveConfigTimeoutMs}},
                                                                {HOLD_BUTTON_TIMEOUT_MS, {holdButtonTimeoutMs}},
                                                                {BUTTON_PIN, {buttonPin}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONFIG] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== other ==========================================================*/
}
