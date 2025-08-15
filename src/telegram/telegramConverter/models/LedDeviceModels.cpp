#include "LedDeviceModels.h"

namespace telegram
{
    namespace UpdateLedDeviceKey
    {
        constexpr const char *COLOR = "color";
        constexpr const char *STATUS = "status";
        inline const std::map<String, std::regex> keys = {
            {COLOR, std::regex("^#[0-9A-Fa-f]{6}$")},
            {STATUS, std::regex("^(on|off)$")}};
    }

    int UpdateLedDevice::countKey() const
    {
        return int(UpdateLedDeviceKey::keys.size());
    }

    UpdateLedDevice::UpdateLedDevice(const String &color, const bool &status, const ModelBaseRequest &obj) : ModelBaseRequest(obj), color(color), status(status) {}

    std::unique_ptr<MessageConvertible> UpdateLedDevice::fromMessage(const su::Text pairs[], const int pairsLength)
    {
        auto parseBasePtr = ModelBaseRequest::fromMessage(pairs, pairsLength);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        using namespace UpdateLedDeviceKey;
        std::map<String, String> map;
        if (auto parseUpdateLedDevicePtr = MessageParse::parsePairs(pairs, pairsLength, keys, map))
        {
            return parseUpdateLedDevicePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());
        return std::make_unique<MessageParseSuccess<UpdateLedDevice>>(UpdateLedDevice(map[COLOR], map[STATUS] == "on", successParseBasePtr->result));
    }

    // String UpdateLedDevice::toMessage() const
    // {
    //     using namespace UpdateLedDeviceKey;
    //     std::vector<std::pair<String, String>> pairs = {
    //         {COLOR, color},
    //         {STATUS, status ? "on" : "off"},
    //     };
    //     return BaseModelRequest::toMessage() + ';' + MessageSerialization::serializationPairs(pairs);
    // }
}