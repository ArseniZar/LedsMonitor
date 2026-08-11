#pragma once
#ifndef API_SERIALIZATION_H
#define API_SERIALIZATION_H

#include <Arduino.h>
#include <GSON.h>
#include "ApiResponse.h"

namespace api
{
    template <typename T>
    gson::Str serializeResponse(const SuccessResponse<T> &response);
    gson::Str serializeResponse(const ErrorResponse &response);

}
#include "ApiSerialization.tpp"

#endif // API_SERIALIZATION_H
