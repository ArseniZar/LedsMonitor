#pragma once
#ifndef JSON_SERIALIZABLE_H
#define JSON_SERIALIZABLE_H

#include <Arduino.h>
#include <GSON.h>

struct JsonSerializable {
    virtual ~JsonSerializable() = default;
    virtual gson::Str toJson() const = 0;
};

#endif // JSON_SERIALIZABLE_H
