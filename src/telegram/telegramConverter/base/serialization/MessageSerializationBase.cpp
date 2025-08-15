#include "MessageSerializationBase.h"
namespace telegram
{
    String MessageSerialization::serializationPairs(const std::vector<std::pair<String, String>> &pairs)
    {
        String result;
        for (size_t i = 0; i < pairs.size(); ++i)
        {
            result += pairs[i].first + '=' + pairs[i].second;
            if (i < pairs.size() - 1)
            {
                result += ';';
            }
        }
        return result;
    }
}