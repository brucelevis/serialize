#pragma once

#include <rapidjson/document.h>

#include <cmath>

#ifndef PAJLADA_REPORT_ERROR
#define PAJLADA_REPORT_ERROR(x) \
    if (x != nullptr) {         \
        *x = true;              \
    }
#endif

#define PAJLADA_ROUNDING_METHOD_ROUND 0
#define PAJLADA_ROUNDING_METHOD_CEIL 1
#define PAJLADA_ROUNDING_METHOD_FLOOR 2

// valid values: ceil/floor/round
// default: round
#ifndef PAJLADA_ROUNDING_METHOD
#define PAJLADA_ROUNDING_METHOD PAJLADA_ROUNDING_METHOD_ROUND
#endif

namespace pajlada {

namespace detail {

template <typename T1, typename T2>
typename std::enable_if<std::is_integral<T1>::value, T1>::type
Round(T2 value)
{
#if PAJLADA_ROUNDING_METHOD == PAJLADA_ROUNDING_METHOD_ROUND
    return round(value);
#elif PAJLADA_ROUNDING_METHOD == PAJLADA_ROUNDING_METHOD_CEIL
    return ceil(value);
#elif PAJLADA_ROUNDING_METHOD == PAJLADA_ROUNDING_METHOD_FLOOR
    return floor(value);
#else
    static_assert(
        "Invalid rounding method selected in PAJLADA_ROUNDING_METHOD");
#endif
}

template <typename Type, typename std::enable_if<
                             std::is_integral<Type>::value>::type * = nullptr>
Type
GetNumber(const rapidjson::Value &value)
{
    if (value.IsDouble()) {
        return Round<Type>(value.GetDouble());
    }
    if (value.IsFloat()) {
        return Round<Type>(value.GetFloat());
    }
    if (value.IsInt()) {
        return value.GetInt();
    }

    return Type{};
}

}  // namespace detail

}  // namespace pajlada