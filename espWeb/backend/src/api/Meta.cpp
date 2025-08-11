#include "Meta.h"

namespace api
{
    namespace MetaApi
    {
        constexpr const char *REQUESTID = "requestId";
        constexpr const char *TIMESTAMP = "timestamp";
    }

    Meta::Meta(const String &requestId, int timestamp)
        : requestId(requestId), timestamp(timestamp)
    {
    }

    const String &Meta::getRequestId() const
    {
        return requestId;
    }
    int Meta::getTimestamp() const
    {
        return timestamp;
    }

    void Meta::setRequestId(const String &id)
    {
        requestId = id;
    }

    void Meta::setTimestamp(int ts)
    {
        timestamp = ts;
    }
    gson::Str Meta::toJson() const
    {
        using namespace MetaApi;
        gson::Str j;
        j('{');
        j[REQUESTID] = requestId;
        j[TIMESTAMP] = timestamp;
        j('}');
        return j;
    }
    std::unique_ptr<JsonConvertible> Meta::fromJson(const gson::Entry &e)
    {
        using namespace MetaApi;

        if (!e.is(gson::Type::Object))
        {
            return std::make_unique<JsonError>(JsonError{"Meta parsing error: expected an object"});
        }

        if (!(e.has(REQUESTID) && e.has(TIMESTAMP)))
        {
            String missingFields;
            if (!e.has(REQUESTID))
                missingFields += "'" + String(REQUESTID) + "' ";
            if (!e.has(TIMESTAMP))
                missingFields += "'" + String(TIMESTAMP) + "' ";

            return std::make_unique<JsonError>(JsonError{"Meta parsing error: missing field(s): " + missingFields});
        }

        if (!(e[REQUESTID].is(gson::Type::String) && e[TIMESTAMP].is(gson::Type::Int)))
        {
            return std::make_unique<JsonError>(JsonError{"Meta parsing error: incorrect type(s) for fields"});
        }

        return std::make_unique<JsonParseSuccess<Meta>>(JsonParseSuccess<Meta>{
            Meta(String(e[REQUESTID].value()), int(e[TIMESTAMP].value()))});
    }

}
