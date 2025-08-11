#pragma once
#include "ApiParse.h"
namespace api
{
    using namespace ApiRequestConstants;

    template <typename T>
    std::unique_ptr<ApiRequest> parseApiRequest(const String &jsonStr)
    {
        gson::Parser parser;
        parser.parse(su::Text(jsonStr));

        std::unique_ptr<ApiErrorRequest> ptr = validateParser(parser);
        if (ptr != nullptr)
        {
            return ptr;
        }

        const gson::Entry &body = parser[DATA];
        const bool isList = body.type() == gson::Type::Array;
        size_t count = isList ? body.length() : 1;

        std::vector<T> data;
        if (isList)
            data.reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            const gson::Entry &entry = isList ? body[int(i)] : body;
            auto ptr = T::fromJson(entry);
            if (!ptr->isOk())
            {
                auto *error_ptr = static_cast<JsonError *>(ptr.get());
                String detailedMessage = "[parseApiRequest] Error parsing '" + String(DATA) + "'";
                if (isList)
                    detailedMessage += " at index " + String(i);
                detailedMessage += ": " + error_ptr->message;
                return std::make_unique<ApiErrorRequest>(std::move(detailedMessage));
            }
            data.push_back(static_cast<JsonParseSuccess<T> *>(ptr.get())->result);
        }

        if (isList)
        {
            auto result = std::make_unique<ApiSuccessRequestList<T>>();
            result->data = std::move(data);
            return result;
        }
        else
        {
            auto result = std::make_unique<ApiSuccessRequestSingle<T>>();
            result->data = std::move(data.front());
            return result;
        }
    }
}
