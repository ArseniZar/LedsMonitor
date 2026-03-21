#pragma once
#ifndef API_MODELS_BASE_H
#define API_MODELS_BASE_H

#include <Arduino.h>
#include "JsonParseBase.h"
#include "JsonSerializationBase.h"

namespace api
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public json::Parse
    {
    public:
        ModelBaseRequest();
        ModelBaseRequest(ModelBaseRequest &&base);
        ModelBaseRequest(const ModelBaseRequest &base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);

        // const String32 meta;
        // ModelBaseRequest() = delete;
        // ModelBaseRequest(const char *meta);
    };

    /*==========================================================ModelResponse==============================================*/

    class ModelBaseResponse : public json::Serialization
    {
    public:
        ModelBaseResponse();
        ModelBaseResponse(ModelBaseResponse &&base);
        ModelBaseResponse(ModelBaseRequest &&base);
        ModelBaseResponse(const ModelBaseResponse &base);
        ModelBaseResponse(const ModelBaseRequest &base);
        gson::Str toJson() const override;

        // const String32 meta;
        // ModelBaseResponse() = delete;
        // ModelBaseResponse(const char *meta);
    };
}

#endif // API_MODELS_BASE_H