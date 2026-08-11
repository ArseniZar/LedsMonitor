#pragma once
#include <optional>
#include <type_traits>
#include <variant>
#include <tuple>

template <typename T>
struct is_variant : std::false_type {};

template <typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template <typename T, typename Pair>
struct has_type_in_pair : std::false_type {};

template <typename T, template<typename U1, typename U2> class Pair, typename U1, typename U2>
struct has_type_in_pair<T, Pair<U1, U2>>: std::is_same<T, U1> {}; 

template <typename T, typename Tuple>
struct has_type_in_tuple_with_pair : std::false_type {};

template <typename T, typename... Args>
struct has_type_in_tuple_with_pair<T, std::tuple<Args...>> {
    static constexpr bool value = (has_type_in_pair<T, Args>::value || ...);
};

template <typename T, template<typename U> class Wrapped>
void fromOptional(const std::optional<T>& opt,  Wrapped<T>& target) {
    if (opt.has_value()) {
        target = *opt;
    }
}

template <typename T>
void fromOptional(const std::optional<T>& opt,  T& target) {
    if (opt.has_value()) {
        target = *opt;
    }
}

struct StrCompare {bool operator()(const char* a, const char* b) const { return strcmp(a, b) < 0; }};



