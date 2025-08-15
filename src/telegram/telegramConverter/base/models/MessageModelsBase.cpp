#include "MessageModelsBase.h"

namespace telegram
{
    /* ================================================ModelBaseRequest================================================*/

    namespace ModelsBaseRequestResponceKey
    {
        constexpr const char *COMMAND = "command";
        constexpr const char *ID = "id";

        inline const std::map<String, std::regex> keys = {
            {COMMAND, std::regex("^[a-z]+$")},
            {ID, std::regex("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$")}};
    }

    int ModelBaseRequest::countKey() const
    {
        return int(ModelsBaseRequestResponceKey::keys.size());
    }
    
    ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &other) : command(other.command), id(other.id) {}
    ModelBaseRequest::ModelBaseRequest(const String &command, const String &id) : command(command), id(id) {}

    std::unique_ptr<MessageConvertible> ModelBaseRequest::fromMessage(const su::Text pairs[], const int pairsLength)
    {

        using namespace ModelsBaseRequestResponceKey;
        std::map<String, String> map;
        if (auto parseBasePtr = MessageParse::parsePairs(pairs, pairsLength, keys, map))
        {
            return parseBasePtr;
        }
        return std::make_unique<MessageParseSuccess<ModelBaseRequest>>(ModelBaseRequest(map[COMMAND], map[ID]));
    }

    /* ================================================ModelBaseResponse================================================*/

    ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &other) : command(other.command), id(other.id) {}
    ModelBaseResponse::ModelBaseResponse(const String &command, const String &id) : command(command), id(id) {}
    
    String ModelBaseResponse::toMessage() const
    {
        using namespace ModelsBaseRequestResponceKey;
        std::vector<std::pair<String, String>> pairs = {
            {COMMAND, command},
            {ID, id},
        };
        return MessageSerialization::serializationPairs(pairs);
    }
}