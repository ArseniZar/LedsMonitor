#pragma once
#ifndef LED_DEVICE_MODELS_H
#define LED_DEVICE_MODELS_H

#include <Arduino.h>
#include "MessageModelsBase.h"

namespace telegram
{
    class UpdateLedDevice final : public ModelBaseRequest
    {
    public:
        String color;
        bool status;
        UpdateLedDevice() = delete;
        int countKey() const override;
        UpdateLedDevice(const String &color, const bool &status, const ModelBaseRequest &obj);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text pairs[], const int pairsLength);
    };
}

#endif // LED_DEVICE_MODELS_H