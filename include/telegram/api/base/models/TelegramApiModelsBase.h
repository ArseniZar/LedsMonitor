#pragma once
#ifndef TELEGRAM_API_MODELS_BASE_H
#define TELEGRAM_API_MODELS_BASE_H

#include <Arduino.h>
#include "JsonParseBase.h"
#include "JsonSerializationBase.h"

namespace api::telegram
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public json::Parse
    {
    public:
        String32 command;
        StringN<18> id; 
        ModelBaseRequest() = delete;
        ModelBaseRequest(ModelBaseRequest &&base);
        ModelBaseRequest(const ModelBaseRequest &base);
        ModelBaseRequest(const char *command, const char *id);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*==========================================================ModelResponse==============================================*/

    class ModelBaseResponse : public json::Serialization
    {
    public:
        String32 command;
        StringN<18> id;
        ModelBaseResponse() = delete;
        ModelBaseResponse(ModelBaseResponse &&base);
        ModelBaseResponse(ModelBaseRequest &&base);
        ModelBaseResponse(const ModelBaseResponse &base);
        ModelBaseResponse(const ModelBaseRequest &base);
        ModelBaseResponse(const char *command, const char *id);
        gson::Str toJson() const override;
    };
}

#endif // MESSAGE_MODELS_BASE_H