#pragma once
#include "ApiParse.h"

namespace api
{

    template <typename T>
    std::unique_ptr<Request> parseRequest(const char *message)
    {

        gson::Parser parser;
        parser.parse(message);

        if (parser.hasError())
        {
            String128 buf;
            buf.add(F("Api Parsing error: at index "));
            buf.add(parser.errorIndex());
            buf.add(F(": "));
            buf.add(parser.readError());
            return std::make_unique<ErrorRequest>(buf);
        }
        
        const gson::Entry &json = parser.get(F("data"));
        if (!json.valid())
        {   
            return std::make_unique<ErrorRequest>(String64(F("Api Parsing error: expected key 'data' not found")));
        }

        auto parsePtr = T::fromJson(json);
        if (!parsePtr->isOk())
        {
            json::Error *errorPtr = static_cast<json::Error *>(parsePtr.get());
            return std::make_unique<ErrorRequest>(errorPtr->message);
        }

        json::ParseSuccess<T> *successPtr = static_cast<json::ParseSuccess<T> *>(parsePtr.get());
        return std::make_unique<SuccessRequest<T>>(std::move(successPtr->result));
    }
}
