#pragma once
#ifndef API_SERIALIZATION_H
#define API_SERIALIZATION_H

#include <Arduino.h>
#include <vector>
#include <concepts>
#include <GSON.h>
#include <memory>
#include "ApiTypes.h"

namespace api
{
    template <typename T>
    String serializeApiResponse(const ApiSuccessResponseSingle<T> &response);

    template <typename T>
    String serializeApiResponse(const ApiSuccessResponseList<T> &response);

    template <typename T>
    String serializeApiResponse(const ApiResponse &response);

    String serializeApiResponse(const ApiErrorResponse &response);

}
#include "ApiSerialization.tpp"

#endif // API_SERIALIZATION_H
