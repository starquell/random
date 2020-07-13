#pragma once

#include "Utils.hpp"
#include "TypeTraits/GeneralTraits.hpp"

#include <variant>

namespace stq {

    /**
     *  @brief Represents schema for built-in arithmetic types that distributes by dist for random generator
     */
    template <typename Distribution = detail::NoDistribution>
    struct Arithmetic {

        explicit constexpr Arithmetic() = default;

        explicit constexpr Arithmetic(Distribution&& dist)
            : _dist{dist}
        {}

        using Dist = Distribution;

        constexpr auto distribution() const noexcept -> Distribution
        {
            return _dist;
        }

    private:
        Distribution _dist{};
    };

    /**
     *  @brief Represents schema for STL-like containers and C arrays for random generator
     */
    template <typename Value>
    class Container {
    public:

        explicit constexpr Container (Value&& value, std::size_t size)
            : _value{value},
              _size{size}
        {}

        constexpr auto value() const noexcept -> const Value&
        {
            return _value;
        }

        constexpr auto size() const noexcept -> std::size_t
        {
            return _size;
        }

    private:
        Value _value;
        std::size_t _size{};
    };

    /**
     *  @brief Represents schema for objects with nested objects, like std::pair, std::tuple, std::variant
     */
    template <typename... Values>
    class Compound {
    public:

        explicit constexpr Compound(Values&&... values)
                : _values{values...}
        {}

        constexpr auto values() const noexcept -> const std::tuple<Values...>&
        {
            return _values;
        }

    private:
        std::tuple<Values...> _values;
    };

    /**
     *  @brief Used for generation default schema for some type
     */
    class Default {};
}