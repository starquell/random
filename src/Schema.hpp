#pragma once

#include "Utils.hpp"

#include <variant>

namespace stq {

    template <typename Distribution = detail::NoDistribution>
    struct Arithmetic {

        explicit Arithmetic() = default;

        explicit Arithmetic(Distribution&& dist)
            : _dist{dist}
        {}

        using Dist = Distribution;

        auto distribution() const noexcept -> Distribution
        {
            return _dist;
        }

    private:
        Distribution _dist{};
    };

    template <typename Value>
    class Container {
    public:

        explicit Container (Value&& value, std::size_t size)
            : _value{value},
              _size{size}
        {}

        auto value() const noexcept -> const Value&
        {
            return _value;
        }

        auto size() const noexcept -> std::size_t
        {
            return _size;
        }

    private:
        Value _value;
        std::size_t _size{};
    };


    template <typename... Values>
    class Compound {
    public:

        explicit Compound(Values&&... values)
                : _values{values...}
        {}

        auto values() const noexcept -> const std::tuple<Values...>&
        {
            return _values;
        }

    private:
        std::tuple<Values...> _values;
    };

}