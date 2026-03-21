#pragma once
#ifndef API_PARSE_H
#define API_PARSE_H

#include <Arduino.h>
#include <GSON.h>
#include <memory>
#include "ApiRequest.h"

namespace api
{
    template <typename T>
    std::unique_ptr<Request> parseRequest(const char* message);
}

#include "ApiParse.tpp"

#endif // API_PARSE_H
