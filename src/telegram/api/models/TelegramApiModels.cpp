#include "TelegramApiModels.h"

namespace api::telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    namespace UpdateLedDeviceRequestKey
    {
        constexpr const char *COLOR = "color";
        constexpr const char *STATUS = "status";
    }

    UpdateLedDeviceRequest::UpdateLedDeviceRequest(const char *color, const bool status, const ModelBaseRequest &base) : ModelBaseRequest(base), color(color), status(status) {}
    UpdateLedDeviceRequest::UpdateLedDeviceRequest(const char *color, const bool status, ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)), color(color), status(status) {}

    std::unique_ptr<json::Convertible> UpdateLedDeviceRequest::fromJson(const gson::Entry &json)
    {
        using namespace UpdateLedDeviceRequestKey;
        static const std::map<const char *, Field> keys = {
            {COLOR, {json::ValueType::String8}},
            {STATUS, {json::ValueType::Bool}}};

        auto parseBasePtr = ModelBaseRequest::fromJson(json);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseUpdateLedDevicePtr = json::Parse::parse(json, keys);
        if (!parseUpdateLedDevicePtr->isOk())
        {
            return parseUpdateLedDevicePtr;
        }
        auto *successParseUpdateLedDevicePtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value>> *>(parseUpdateLedDevicePtr.get());
        std::map<const char *, json::Value> map = std::move(successParseUpdateLedDevicePtr->result);

        const String8 &color = std::get<String8>(map[COLOR]);
        const bool status = std::get<bool>(map[STATUS]);

        return std::make_unique<json::ParseSuccess<UpdateLedDeviceRequest>>(std::move(UpdateLedDeviceRequest(color.c_str(), status, std::move(successParseBasePtr->result))));
    }

    /*===================================== GetInfoLedDevice ==========================================================*/

    namespace GetInfoLedDeviceResponseKey
    {
        constexpr const char *NAME = "name";
        constexpr const char *INFO = "info";
    }

    GetInfoLedDeviceResponse::GetInfoLedDeviceResponse(const char *name, const ModelBaseResponse &base) : ModelBaseResponse(base), name(name) {}
    GetInfoLedDeviceResponse::GetInfoLedDeviceResponse(const char *name, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), name(name) {}

    gson::Str GetInfoLedDeviceResponse::toJson() const
    {
        using namespace GetInfoLedDeviceResponseKey;
        std::array<std::pair<const char *, Field>, 1> pairs = {{
            {NAME, {name}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[INFO] = json::Serialization::serialization(pairs);
        return j;
    }

    /*===================================== GetStateLedDevice ==========================================================*/


    namespace GetStateLedDeviceResponseKey
    {
        constexpr const char *COLOR = "color";
        constexpr const char *STATUS = "status";
        constexpr const char *STATE = "state";
    }

    GetStateLedDeviceResponse::GetStateLedDeviceResponse(const char *color, const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), color(color), status(status) {}
    GetStateLedDeviceResponse::GetStateLedDeviceResponse(const char *color, const bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), color(color), status(status) {}

    gson::Str GetStateLedDeviceResponse::toJson() const
    {
        using namespace GetStateLedDeviceResponseKey;
        std::array<std::pair<const char *, Field>, 2> pairs = {{
            {COLOR, {color}},
            {STATUS, {status}}}};
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATE] = json::Serialization::serialization(pairs);
        return j;
    }

    /*========================================= other ==========================================================*/
}