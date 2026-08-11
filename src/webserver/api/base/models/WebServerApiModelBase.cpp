#include "WebServerApiModelsBase.h"

namespace api::webserver
{
    /* ================================================ModelBaseRequest================================================*/

    namespace ModelsBaseRequestResponceKey
    {
        // constexpr const char *META = "meta";
    }

    ModelBaseRequest::ModelBaseRequest() {}
    ModelBaseRequest::ModelBaseRequest(ModelBaseRequest &&base) {}
    ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &base) {}

    std::unique_ptr<json::Convertible> ModelBaseRequest::fromJson(const gson::Entry &json)
    {
        return std::make_unique<json::ParseSuccess<ModelBaseRequest>>(std::move(ModelBaseRequest()));
    }

    /* ================================================ModelBaseResponce================================================*/

    ModelBaseResponse::ModelBaseResponse() {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseResponse &&base) {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseRequest &&base) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &base) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseRequest &base) {}

    gson::Str ModelBaseResponse::toJson() const
    {
        gson::Str j;
        return j;
    }
}