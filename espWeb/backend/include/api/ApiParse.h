#pragma once
#ifndef API_PARSE_H
#define API_PARSE_H

#include <Arduino.h>
#include <vector>
#include <concepts>
#include <GSON.h>
#include <memory>
#include "ApiTypes.h"
#include "JsonTypes.h"

namespace api
{
    template <typename T>
    std::unique_ptr<ApiRequest> parseApiRequest(const String &jsonStr);

    std::unique_ptr<ApiErrorRequest> validateParser(const gson::Parser &parser);
}
#include "ApiParse.tpp"

#endif // API_PARSE_H
