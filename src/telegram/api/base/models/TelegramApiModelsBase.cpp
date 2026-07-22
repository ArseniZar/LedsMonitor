#include "TelegramApiModelsBase.h"

namespace api::telegram
{
    namespace ModelsBaseRequestResponceKey
    {
        constexpr const char *COMMAND = "command";
        constexpr const char *ID = "id";
    }

    /* ================================================ ModelBaseRequest ================================================*/

    ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &base) : command(base.command), id(base.id) {}
    ModelBaseRequest::ModelBaseRequest(ModelBaseRequest &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseRequest::ModelBaseRequest(const char *command, const char *id) : command(command), id(id) {}
    
    std::unique_ptr<json::Convertible> ModelBaseRequest::fromJson(const gson::Entry &json)
    {
        using namespace ModelsBaseRequestResponceKey;
        static const std::map<const char *, Field, StrCompare> keys = {
            {COMMAND, {json::ValueType::String32}},
            {ID, {json::ValueType::String18}}};

        auto parseBasePtr = json::Parse::parse(json, keys);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<json::ParseSuccess<std::map<const char *, json::Value, StrCompare>> *>(parseBasePtr.get());
        std::map<const char *, json::Value, StrCompare> parseMap = std::move(successParseBasePtr->result);

        const String32 &command = std::get<String32>(parseMap[COMMAND]);
        const StringN<18> &id = std::get<StringN<18>>(parseMap[ID]);

        return std::make_unique<json::ParseSuccess<ModelBaseRequest>>(std::move(ModelBaseRequest(command.c_str(), id.c_str())));
    }

    /* ================================================ ModelBaseResponse ===============================================*/

    ModelBaseResponse::ModelBaseResponse(ModelBaseResponse &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseRequest &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &base) : command(base.command), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseRequest &base) : command(base.command), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const char *command, const char *id) : command(command), id(id) {}

    gson::Str ModelBaseResponse::toJson() const
    {
        using namespace ModelsBaseRequestResponceKey;
        std::array<std::pair<const char *, Field>, 2> pairs = {{
            {COMMAND,{command}},
            {ID,{id}}}};
        return Serialization::serialization(pairs);
    }
} 