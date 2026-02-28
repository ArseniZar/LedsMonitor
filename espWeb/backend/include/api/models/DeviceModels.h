#pragma once
#ifndef DEVICE_MODELS_H
#define DEVICE_MODELS_H

#include <Arduino.h>
#include "JsonModelBase.h"
#include "WifiNetwork.h"
#include "WifiStates.h"

namespace api
{
    /*===================================== ScanNetworkStarted =======================================================*/

    class ScanNetworkStartedResponce final : public ModelBaseResponse
    {
    public:
        bool status;
        ScanNetworkStartedResponce() = delete;
        ScanNetworkStartedResponce(const bool status, const ModelBaseResponse &base);
        ScanNetworkStartedResponce(bool status,ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== ScanStatus =======================================================*/

    class ScanStatusResponce final : public ModelBaseResponse
    {
    public:
        ScanState status;
        ScanStatusResponce() = delete;
        ScanStatusResponce(const ScanState &status, const ModelBaseResponse &base);
        ScanStatusResponce(ScanState &&status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };
    
    /*===================================== ScanNetwork =======================================================*/

    // class ScanNetworkRequest final : public ModelBaseRequest
    // {
    // public:
    //     ScanNetworkRequest() = delete;
    //     ScanNetworkRequest(ModelBaseRequest &&base);
    //     ScanNetworkRequest(const ModelBaseRequest &base);
    //     static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &data);
    // };
    
    class ScanNetworkResponce final : public ModelBaseResponse
    {
    public:
        std::vector<WifiNetwork> networks;
        ScanNetworkResponce() = delete;
        ScanNetworkResponce(const std::vector<WifiNetwork> &networks, const ModelBaseResponse &base);
        ScanNetworkResponce(std::vector<WifiNetwork> &&networks, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== ConnectNetwork ====================================================*/

    class ConnectNetworkRequest final : public ModelBaseRequest
    {
    public:
        WifiNetwork network;
        ConnectNetworkRequest() = delete;
        ConnectNetworkRequest(WifiNetwork &&network, ModelBaseRequest &&base);
        ConnectNetworkRequest(const WifiNetwork &network, const ModelBaseRequest &base);
        static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &data);
    };

    class ConnectNetworkResponce final : public ModelBaseResponse
    {
    public:
        bool status;
        ConnectNetworkResponce() = delete;
        ConnectNetworkResponce(const bool status, const ModelBaseResponse &base);
        ConnectNetworkResponce(bool status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== StatusWifi =======================================================*/

    class WifiStatusResponce final : public ModelBaseResponse
    {
    public:
        ConnState status;
        WifiStatusResponce() = delete;
        WifiStatusResponce(const ConnState &status, const ModelBaseResponse &base);
        WifiStatusResponce(ConnState &&status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    


    /*===================================== other =======================================================*/
}
#endif // DEVICE_MODELS_H