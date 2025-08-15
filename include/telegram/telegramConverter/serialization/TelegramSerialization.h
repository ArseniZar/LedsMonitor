#pragma once
#ifndef TELEGRAM_SERIALIZATION_H
#define TELEGRAM_SERIALIZATION_H

#include <Arduino.h>
#include "TelegramTypesBase.h"
#include "TelegramResponse.h"

namespace telegram
{
    template <typename T>
    String serializationTelegramResponce(const TelegramSuccessResponse<T> &responce);
    String serializationTelegramResponce(const TelegramErrorResponse &response);
}

#include "TelegramSerialization.tpp"
#endif // TELEGRAM_SERIALIZATION_H