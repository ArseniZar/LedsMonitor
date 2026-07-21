#pragma once
#ifndef TELEGRAM_API_MODELS_H
#define TELEGRAM_API_MODELS_H

#include <Arduino.h>
#include "TelegramApiModelsBase.h"

namespace api::telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    class UpdateLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        StringN<8> color;
        bool status;
        UpdateLedDeviceRequest() = delete;
        UpdateLedDeviceRequest(const char *color, const bool status, const ModelBaseRequest &base);
        UpdateLedDeviceRequest(const char *color, const bool status, ModelBaseRequest &&base);
        static std::unique_ptr<json::Convertible> fromJson(const gson::Entry &json);
    };

    /*===================================== GetInfoLedDevice =========================================================*/

    class GetInfoLedDeviceResponse final : public ModelBaseResponse
    {
    public:
        String32 name;
        GetInfoLedDeviceResponse() = delete;
        GetInfoLedDeviceResponse(const char *name, const ModelBaseResponse &base);
        GetInfoLedDeviceResponse(const char *name,  ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== GetLedDevice =========================================================*/

    class GetStateLedDeviceResponse final : public ModelBaseResponse
    {
    public:
        StringN<8> color;
        bool status;
        GetStateLedDeviceResponse() = delete;
        GetStateLedDeviceResponse(const char *color, const bool status, const ModelBaseResponse &base);
        GetStateLedDeviceResponse(const char *color, const bool status,  ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };
    /*===================================== ************ =========================================================*/
}

#endif // LED_DEVICE_MODELS_H