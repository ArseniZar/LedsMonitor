#pragma once
#include "TelegramSerialization.h"

namespace telegram
{
    template <typename T>
    String serializationTelegramResponce(const TelegramSuccessResponse<T> &responce)
    {
        String result;
        result += '/';
        result += responce.data.toMessage();
        result += '/';
        return result;
    }

    
}