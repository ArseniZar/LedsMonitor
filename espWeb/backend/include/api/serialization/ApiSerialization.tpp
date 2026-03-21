#pragma once
#include "ApiSerialization.h"

namespace api
{
    template <typename T>
    String serializeResponse(const SuccessResponse<T> &response)
    {
        gson::Str j;
        j('{');
        j[F("ok")] = response.isOk();
        j[F("result")]('{');
        j += response.data.toJson();
        j('}');
        j('}');
        return String(su::Text(j).c_str());
    }
}