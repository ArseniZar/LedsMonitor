#pragma once
#ifndef WEBSERVER_API_MODELS_BASE_H
#define WEBSERVER_API_MODELS_BASE_H

#include <Arduino.h>
#include "JsonParseBase.h"
#include "JsonSerializationBase.h"

namespace api::webserver
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public json::Parse
    {
    public:
        ModelBaseRequest();
        ModelBaseRequest(ModelBaseRequest &&base);
        ModelBaseRequest(const ModelBaseRequest &base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
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
    };
}

#endif // WEBSERVER_API_MODELS_BASE_H