#include "ApiSerialization.h"

namespace api
{   
    using namespace ApiResponseConstants;
    String serializeApiResponse(const ApiErrorResponse &response)
    {
        gson::Str j;
        j('{');
        j[OK] = response.ok;
        j[ERROR_CODE] = response.error_code;
        j[MESSAGE] = response.message;
        j('}');
        return String(su::Text(j));
    }

}
