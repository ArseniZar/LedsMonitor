#include "ApiSerialization.h"

namespace api
{
    gson::Str serializeResponse(const ErrorResponse &response)
    {

        gson::Str j;
        j('{');
        j[F("ok")] = response.isOk();
        j[F("error_code")] = response.getCode();
        j[F("message")] = response.message;
        j('}');
        return j;
    }

}
