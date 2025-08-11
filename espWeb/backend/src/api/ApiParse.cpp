#include "ApiParse.h"

namespace api
{
    std::unique_ptr<ApiErrorRequest> validateParser(const gson::Parser &parser)
    {
        using namespace api::ApiRequestConstants;

        if (parser.hasError())
        {
            return std::make_unique<ApiErrorRequest>("Parsing error at index " + String(parser.errorIndex()) + ": " + parser.readError());
        }

        const gson::Entry &bodyEntry = parser[DATA];
        const bool bodyData = bodyEntry.valid();

        if (!bodyData)
        {
            return std::make_unique<ApiErrorRequest>("Parsing error: missing critical field(s): '" + String(DATA) + "' ");
        }

        const gson::Type bodyType = bodyEntry.type();
        if (bodyType != gson::Type::Array && bodyType != gson::Type::Object)
        {
            return std::make_unique<ApiErrorRequest>("Parsing error: '" + String(DATA) + "' must be an object or array.");
        }
        return nullptr;
    }
}
