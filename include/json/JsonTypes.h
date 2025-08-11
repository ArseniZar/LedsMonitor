#pragma once
#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <Arduino.h>
#include <GSON.h>

struct JsonConvertible {
    virtual bool isOk() const { return true; }
    virtual ~JsonConvertible() = default;
};

template <typename T>
struct JsonParseSuccess : public JsonConvertible {
    T result;

    JsonParseSuccess(const T& obj) : result(obj) {}
};

struct JsonError : public JsonConvertible {
    String message;

    JsonError(const String& msg) : message(msg) {}

    bool isOk() const override {
        return false;
    }
};

#endif // JSON_TYPES_H
