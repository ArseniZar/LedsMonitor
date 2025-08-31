#pragma once
#ifndef MESSAGE_MODELS_BASE_H
#define MESSAGE_MODELS_BASE_H

#include <Arduino.h>
#include "MessageParseBase.h"
#include "MessageSerializationBase.h"

namespace telegram
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public MessageParse
    {
    public:
        String command;
        String id;
        ModelBaseRequest() = delete;
        int countKey() const override;
        ModelBaseRequest(const ModelBaseRequest &other);
        ModelBaseRequest(const String &command, const String &id);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text pairs[], const int pairsLength);
    };

    /*==========================================================ModelResponse==============================================*/

    class ModelBaseResponse : public MessageSerialization
    {
    public:
        String command;
        String id;
        ModelBaseResponse() = delete;
        String toMessage() const override;
        ModelBaseResponse(const ModelBaseResponse &other);
        ModelBaseResponse(const ModelBaseRequest &other);
        ModelBaseResponse(const String &command, const String &id);
    };
}

#endif // MESSAGE_MODELS_BASE_H