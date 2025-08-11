#pragma once
#include "ApiSerialization.h"

namespace api
{
    using namespace ApiResponseConstants;
    template <typename T>
    String serializeApiResponse(const ApiSuccessResponseSingle<T> &response)
    {
        gson::Str j;
        j('{');
        j[OK] = response.ok;
        j[RESULT] = response.result.toJson();
        j('}');
        return String(su::Text(j));
    }

    template <typename T>
    String serializeApiResponse(const ApiSuccessResponseList<T> &response)
    {
        gson::Str j;
        j('{');
        j[OK] = response.ok;
        j[RESULT]('[');
        for (const T &item : response.result)
        {   
            j += item.toJson();
        }
        j(']');
        j('}');
        return String(su::Text(j));
    }

    template <typename T>
    String serializeApiResponse(const ApiResponse &response)
    {
        switch (response.type())
        {
        case ApiResponseType::SuccessList:
            return serializeApiResponse(static_cast<const ApiSuccessResponseList<T> &>(response));
        case ApiResponseType::SuccessSingle:
            return serializeApiResponse(static_cast<const ApiSuccessResponseSingle<T> &>(response));
        case ApiResponseType::Error:
            return serializeApiResponse(static_cast<const ApiErrorResponse &>(response));

        default:
            ApiErrorResponse error(400, "JSON parsing error: The provided JSON data is invalid or corrupted");
            return serializeApiResponse(error);
        }
    }
}