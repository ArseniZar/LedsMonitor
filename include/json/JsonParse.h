#pragma once
#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include <Arduino.h>
#include <GSON.h>
#include <memory>
#include "JsonTypes.h"

class JsonParse
{
public:
    virtual ~JsonParse() = default;
    static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &e);
};

#endif // JSON_SERIALIZABLE_H
