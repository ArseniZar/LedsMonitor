#pragma once
#include "JsonSerializationBase.h"

namespace api::json
{
    template <std::size_t N>
    gson::Str Serialization::serialization(const std::array<std::pair<const char *, Field>, N> &pairs)
    {
        gson::Str j;
        j('{');
        for (const auto &[key, field] : pairs)
        {
            std::visit([&j, key](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                     // ingnore monostate
                }
                else 
                {
                    j[key] = arg;
                }
            }, field.value);
        }
        j('}');
        return j;
    }
}