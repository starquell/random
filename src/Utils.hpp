#pragma once

#include <utility>
#include <tuple>

namespace strq::detail {

    struct NoDistribution{};

    template <typename...>
    inline constexpr auto always_false = false;


    template <typename... LhsTs, typename... RhsTs, std::size_t... Indexes>
    constexpr auto zip_impl (const std::tuple<LhsTs...>& lhs,
                   const std::tuple<RhsTs...>& rhs,
                   std::index_sequence<Indexes...>) -> std::tuple <std::pair <LhsTs, RhsTs>...>
    {
        return {
                std::pair{std::get<Indexes>(lhs), std::get<Indexes>(rhs)}...
        };
    }

    template <typename... LhsTs, typename... RhsTs>
    constexpr auto zip (const std::tuple<LhsTs...>& lhs, const std::tuple<RhsTs...>& rhs)
                                    -> std::tuple <std::pair <LhsTs, RhsTs>...>
    {
        static_assert(sizeof...(LhsTs) == sizeof...(RhsTs),
                      "Tuples have different length.");
        return zip_impl(lhs, rhs, std::make_index_sequence<sizeof...(LhsTs)>{});
    }


}