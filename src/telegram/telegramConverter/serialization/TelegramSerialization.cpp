#include "TelegramSerialization.h"

namespace telegram
{
    String serializationTelegramResponce(const TelegramErrorResponse &response)
    {
        {
            String result;
            result.reserve(response.text.length()+response.message.length()+11);
            result += '/';
            result += response.text;
            result += ';';
            result += "message=" + response.message;
            result += '/';
            return result;
        }
    }
}