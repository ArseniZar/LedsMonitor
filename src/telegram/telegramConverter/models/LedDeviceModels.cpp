#include "LedDeviceModels.h"

namespace telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    namespace UpdateLedDeviceRequestKey
    {
        constexpr const char *COLOR = "color";
        constexpr const char *STATUS = "status";
        inline const std::map<String, std::regex> keys = {
            {COLOR, std::regex("^#[0-9A-Fa-f]{6}$")},
            {STATUS, std::regex("^(on|off)$")}};
    }

    int UpdateLedDeviceRequest::countKey() const
    {
        return int(UpdateLedDeviceRequestKey::keys.size() + ModelBaseRequest::countKey());
    }

    UpdateLedDeviceRequest::UpdateLedDeviceRequest(const String &color, const bool &status, const ModelBaseRequest &obj) : ModelBaseRequest(obj), color(color), status(status) {}

    std::unique_ptr<MessageConvertible> UpdateLedDeviceRequest::fromMessage(const su::Text pairs[], const int pairsLength)
    {
        auto parseBasePtr = ModelBaseRequest::fromMessage(pairs, pairsLength);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        using namespace UpdateLedDeviceRequestKey;
        std::map<String, String> map;
        if (auto parseUpdateLedDevicePtr = MessageParse::parsePairs(pairs, pairsLength, keys, map))
        {
            return parseUpdateLedDevicePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());
        return std::make_unique<MessageParseSuccess<UpdateLedDeviceRequest>>(UpdateLedDeviceRequest(map[COLOR], map[STATUS] == "on", successParseBasePtr->result));
    }

    /*===================================== ScanLedDevice ==========================================================*/

    namespace ScanLedDeviceRequestKey
    {
        inline const std::map<String, std::regex> keys = {};
    }

    int ScanLedDeviceRequest::countKey() const
    {
        return int(ScanLedDeviceRequestKey::keys.size() + ModelBaseRequest::countKey());
    }

    ScanLedDeviceRequest::ScanLedDeviceRequest(const ModelBaseRequest &obj) : ModelBaseRequest(obj) {}

    std::unique_ptr<MessageConvertible> ScanLedDeviceRequest::fromMessage(const su::Text pairs[], const int pairsLength)
    {
        auto parseBasePtr = ModelBaseRequest::fromMessage(pairs, pairsLength);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        using namespace ScanLedDeviceRequestKey;
        std::map<String, String> map;
        if (auto parseUpdateLedDevicePtr = MessageParse::parsePairs(pairs, pairsLength, keys, map))
        {
            return parseUpdateLedDevicePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());
        return std::make_unique<MessageParseSuccess<ScanLedDeviceRequest>>(ScanLedDeviceRequest(successParseBasePtr->result));
    }

    namespace ScanLedDeviceResponseKey
    {
        constexpr const char *NAME = "name";
    }

    ScanLedDeviceResponse::ScanLedDeviceResponse(const String &name, const ModelBaseResponse &obj) : ModelBaseResponse(obj), name(name) {}

    String ScanLedDeviceResponse::toMessage() const
    {
        using namespace ScanLedDeviceResponseKey;
        std::vector<std::pair<String, String>> pairs = {
            {NAME, name},
        };
        return ModelBaseResponse::toMessage() + ';' + MessageSerialization::serializationPairs(pairs);
    }

    /*========================================= other ==========================================================*/
}