#pragma once
#ifndef API_SERIALIZATION_H
#define API_SERIALIZATION_H

#include <Arduino.h>
#include <GSON.h>
#include "ApiResponse.h"

namespace api
{
    template <typename T>
    String serializeResponse(const SuccessResponse<T> &response);
    String serializeResponse(const ErrorResponse &response);

}
#include "ApiSerialization.tpp"

#endif // API_SERIALIZATION_H
