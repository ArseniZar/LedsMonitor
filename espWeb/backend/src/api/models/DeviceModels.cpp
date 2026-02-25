#include "DeviceModels.h"

namespace api
{   
    /*===================================== ScanNetworkStarted ==========================================================*/

    namespace ScanNetworkStartedResponceKey
    {
        constexpr const char *SCAN = "scan";
        constexpr const char *STARTED = "started";
    }

    ScanNetworkStartedResponce::ScanNetworkStartedResponce(const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    ScanNetworkStartedResponce::ScanNetworkStartedResponce(bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(status) {};

    gson::Str ScanNetworkStartedResponce::toJson() const
    {
        const int size = 1;
        using namespace ScanNetworkStartedResponceKey;
        std::tuple<const char *, gson::Type, String> pairs[size] = {
            {STARTED, gson::Type::Bool, status ? "true" : "false"}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[SCAN] = JsonSerialization::serializationJson(pairs, size);
        return j;
    }

    /*===================================== ScanStatus ==========================================================*/

    namespace ScanStatusResponceKey
    {
        constexpr const char *STATUS = "status";
        constexpr const char *CODE = "code";
    }

    ScanStatusResponce::ScanStatusResponce(const ScanState &status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    ScanStatusResponce::ScanStatusResponce(ScanState &&status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(std::move(status)) {};

    gson::Str ScanStatusResponce::toJson() const
    {
        const int size = 1;
        using namespace ScanStatusResponceKey;
        std::tuple<const char *, gson::Type, String> pairs[size] = {
            {CODE, gson::Type::Int, String(static_cast<int>(status))}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = JsonSerialization::serializationJson(pairs, size);
        return j;
    }

    /*===================================== ScanNetwork ==========================================================*/

    // namespace ScanNetworkRequestKey
    // {

    // }

    // ScanNetworkRequest::ScanNetworkRequest(const ModelBaseRequest &base) : ModelBaseRequest(base) {}
    // ScanNetworkRequest::ScanNetworkRequest(ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)) {}

    // std::unique_ptr<JsonConvertible> ScanNetworkRequest::fromJson(const gson::Entry &data)
    // {
    //     using namespace ScanNetworkRequestKey;
    //     static const std::map<const char *, gson::Type> keys = {};

    //     auto parseBasePtr = ModelBaseRequest::fromJson(data);
    //     if (!parseBasePtr->isOk())
    //     {
    //         return parseBasePtr;
    //     }
    //     auto *successParseBasePtr = static_cast<JsonParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

    //     auto parseScanNetworkPtr = JsonParse::parseData(data, keys);
    //     if (!parseScanNetworkPtr->isOk())
    //     {
    //         return parseScanNetworkPtr;
    //     }
    //     auto *successParseScanNetworkPtr = static_cast<JsonParseSuccess<std::map<const char *, String>> *>(parseScanNetworkPtr.get());
    //     std::map<const char *, String> _ = std::move(successParseScanNetworkPtr->result);
    //     return std::make_unique<JsonParseSuccess<ScanNetworkRequest>>(std::move(ScanNetworkRequest(std::move(successParseBasePtr->result))));
    // }

    /*--------------------------------------------------------------------------------------------------------------*/

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

    ScanNetworkResponce::ScanNetworkResponce(const std::vector<WifiNetwork> &networks, const ModelBaseResponse &base) : ModelBaseResponse(base), networks(networks) {};
    ScanNetworkResponce::ScanNetworkResponce(std::vector<WifiNetwork> &&networks, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), networks(std::move(networks)) {};

    gson::Str ScanNetworkResponce::toJson() const
    {
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[ScanNetworksResponceKey::NETWORKS]('[');

        for (size_t i = 0; i < networks.size(); i++)
        {
            const int size = 7;
            using namespace ScanNetworksResponceKey;
            std::tuple<const char *, gson::Type, String> pairs[size] = {
                {SSID, gson::Type::String, networks[i].ssid.c_str()},
                {PASSWORD, gson::Type::String, networks[i].password.c_str()},
                {RSSI, gson::Type::Int, String(networks[i].rssi)},
                {ENCRYPTION, gson::Type::Int, String(networks[i].encryptionType)},
                {CHANNEL, gson::Type::Int, String(networks[i].channel)},
                {BSSID, gson::Type::String, networks[i].bssid.c_str()},
                {HIDDEN, gson::Type::Bool, networks[i].hidden ? "true" : "false"}};

            j += JsonSerialization::serializationJson(pairs, size);
        }
        j(']');
        return j;
    }

    /*===================================== ConnectNetwork ==========================================================*/

    namespace ConnectNetworkRequestKey
    {
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
        constexpr const char *RSSI = "rssi";
        constexpr const char *ENCRYPTION = "encryptionType";
        constexpr const char *CHANNEL = "channel";
        constexpr const char *BSSID = "bssid";
        constexpr const char *HIDDEN = "hidden";
    }

    ConnectNetworkRequest::ConnectNetworkRequest(const WifiNetwork &network, const ModelBaseRequest &base) : ModelBaseRequest(base), network(network) {}
    ConnectNetworkRequest::ConnectNetworkRequest(WifiNetwork &&network, ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)), network(std::move(network)) {}

    std::unique_ptr<JsonConvertible> ConnectNetworkRequest::fromJson(const gson::Entry &data)
    {
        using namespace ConnectNetworkRequestKey;
        static const std::map<const char *, gson::Type> keys = {
            {SSID, gson::Type::String},
            {PASSWORD, gson::Type::String},
            {RSSI, gson::Type::Int},
            {ENCRYPTION, gson::Type::Int},
            {CHANNEL, gson::Type::Int},
            {BSSID, gson::Type::String},
            {HIDDEN, gson::Type::Bool}};

        auto parseBasePtr = ModelBaseRequest::fromJson(data);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<JsonParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseConnectNetworkPtr = JsonParse::parseData(data, keys);
        if (!parseConnectNetworkPtr->isOk())
        {
            return parseConnectNetworkPtr;
        }
        auto *successParseConnectNetworkPtr = static_cast<JsonParseSuccess<std::map<const char *, String>> *>(parseConnectNetworkPtr.get());
        std::map<const char *, String> map = std::move(successParseConnectNetworkPtr->result);
        return std::make_unique<JsonParseSuccess<ConnectNetworkRequest>>(std::move(ConnectNetworkRequest(std::move(WifiNetwork(map[SSID].c_str(), map[PASSWORD].c_str(), map[RSSI].toInt(), map[ENCRYPTION].toInt(), map[CHANNEL].toInt(), map[BSSID].c_str(), map[HIDDEN].equalsIgnoreCase("true"))), std::move(successParseBasePtr->result))));
    }

    /*--------------------------------------------------------------------------------------------------------------*/

    namespace ConnectNetworkResponceKey
    {
        constexpr const char *CONNECT = "connect";
        constexpr const char *STARTED = "started";
        
    }

    ConnectNetworkResponce::ConnectNetworkResponce(const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {}
    ConnectNetworkResponce::ConnectNetworkResponce(bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(status) {}

    gson::Str ConnectNetworkResponce::toJson() const
    {
        const int size = 1;
        using namespace ConnectNetworkResponceKey;
        std::tuple<const char *, gson::Type, String> pairs[size] = {
            {STARTED, gson::Type::Bool, status ? "true" : "false"}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[CONNECT] = JsonSerialization::serializationJson(pairs, size);
        return j;
    }

    /*===================================== StatusWifi ==========================================================*/

    namespace StatusWifiResponceKey
    {
        constexpr const char *STATUS = "status";
        constexpr const char *CODE = "code";
    }

    WifiStatusResponce::WifiStatusResponce(const ConnState &status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {};
    WifiStatusResponce::WifiStatusResponce(ConnState &&status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(std::move(status)) {};

    gson::Str WifiStatusResponce::toJson() const
    {
        const int size = 1;
        using namespace StatusWifiResponceKey;
        std::tuple<const char *, gson::Type, String> pairs[size] = {
            {CODE, gson::Type::Int, String(static_cast<int>(status))}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = JsonSerialization::serializationJson(pairs, size);
        return j;
    }

    

    /*===================================== other ==========================================================*/
}
