#pragma once
#ifndef JSON_CONVERTER_H
#define JSON_CONVERTER_H

#include <GSON.h>
#include "JsonTypes.h"
#include <optional>
#include <limits>
#include <type_traits>

namespace api::json
{
	class Converter
	{
	public:
		static bool isCompatible(JsonType actualType, ValueType expectedType);
		static Value convert(const char *rawValue, ValueType expectedType);

		template <typename T>
		static std::optional<T> toOptional(const Value &value);
	};
}

#include "JsonConverter.tpp"
#endif // JSON_CONVERTER_H
