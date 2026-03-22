#include "ApiModelsBase.h"

namespace api
{
    /* ================================================ModelBaseRequest================================================*/

    namespace ModelsBaseRequestResponceKey
    {
        // constexpr const char *META = "meta";
    }

    ModelBaseRequest::ModelBaseRequest() {}
    ModelBaseRequest::ModelBaseRequest(ModelBaseRequest &&base) {}
    ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &base) {}

    // ModelBaseRequest::ModelBaseRequest(const char *meta) : meta(meta) {}
    // ModelBaseRequest::ModelBaseRequest(ModelBaseRequest &&base) : meta(std::move(base.meta)) {}
    // ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &base) : meta(base.meta) {}

    std::unique_ptr<json::Convertible> ModelBaseRequest::fromJson(const gson::Entry &json)
    {
        return std::make_unique<json::ParseSuccess<ModelBaseRequest>>(std::move(ModelBaseRequest()));

        // using namespace ModelsBaseRequestResponceKey;
        // static const std::map<const char *, gson::Type> keys = {
        //     {META, gson::Type::String}};

        // auto parseBasePtr = JsonParse::parseData(data, keys);
        // if (!parseBasePtr->isOk())
        // {
        //     return parseBasePtr;
        // }
        // auto *successParseBasePtr = static_cast<JsonParseSuccess<std::map<const char *, String>> *>(parseBasePtr.get());
        // std::map<const char *, String> map = std::move(successParseBasePtr->result);
        // return std::make_unique<JsonParseSuccess<ModelBaseRequest>>(std::move(ModelBaseRequest(map[META].c_str())));
    }

    /* ================================================ModelBaseResponce================================================*/

    ModelBaseResponse::ModelBaseResponse() {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseResponse &&base) {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseRequest &&base) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &base) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseRequest &base) {}

    // ModelBaseResponse::ModelBaseResponse(const char *meta) : meta(meta) {}
    // ModelBaseResponse::ModelBaseResponse(ModelBaseResponse &&base) : meta(std::move(base.meta)) {}
    // ModelBaseResponse::ModelBaseResponse(ModelBaseRequest &&base) : meta(std::move(base.meta)) {}
    // ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &base) : meta(base.meta) {}
    // ModelBaseResponse::ModelBaseResponse(const ModelBaseRequest &base) : meta(base.meta) {}

    gson::Str ModelBaseResponse::toJson() const
    {
        gson::Str j;
        return j;
        // const int size = 1;
        // using namespace ModelsBaseRequestResponceKey;
        // std::tuple<const char *, gson::Type, String> pairs[size] = {
        //     {META, gson::Type::String, meta.c_str()}};

        // gson::Str j;
        // j[META] = JsonSerialization::serializationJson(pairs, size);
        // return j;
    }
}