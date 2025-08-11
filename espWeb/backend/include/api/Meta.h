#pragma once
#ifndef META_H
#define META_H

#include <Arduino.h>
#include "JsonSerializable.h"
#include "JsonParse.h"
#include "JsonTypes.h"

namespace api
{

    class Meta : public JsonSerializable, JsonParse
    {
    public:
        String requestId;
        int timestamp;

        Meta(const String &requestId, int timestamp);
        Meta() = default;
        const String &getRequestId() const;
        int getTimestamp() const;

        void setRequestId(const String &id);
        void setTimestamp(int ts);

        gson::Str toJson() const override;
        static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &e);

    private:
    };

}

#endif // META_H
