#pragma once
#ifndef LED_DEVICE_MODELS_H
#define LED_DEVICE_MODELS_H

#include <Arduino.h>
#include "MessageModelsBase.h"

namespace telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    class UpdateLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        String color;
        bool status;
        UpdateLedDeviceRequest() = delete;
        int countKey() const override;
        UpdateLedDeviceRequest(const String &color, const bool &status, const ModelBaseRequest &obj);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text pairs[], const int pairsLength);
    };

    /*===================================== ScanLedDevice =========================================================*/

    class ScanLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        ScanLedDeviceRequest() = delete;
        int countKey() const override;
        ScanLedDeviceRequest(const ModelBaseRequest &obj);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text pairs[], const int pairsLength);
    };

    class ScanLedDeviceResponse final : public ModelBaseResponse
    {
    public:
        String name;
        ScanLedDeviceResponse() = delete;
        String toMessage() const override;
        ScanLedDeviceResponse(const String &name, const ModelBaseResponse &obj);
    };
}

#endif // LED_DEVICE_MODELS_H