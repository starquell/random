#pragma once

#include <utility>
#include <tuple>

namespace stq::detail {

    struct NoDistribution{};

    template <typename...>
    inline constexpr auto always_false = false;

    template <typename... LhsTs, typename... RhsTs, std::size_t... Indexes>
    auto zip_impl (const std::tuple<LhsTs...>& lhs,
                   const std::tuple<RhsTs...>& rhs,
                   std::index_sequence<Indexes...>) -> std::tuple <std::pair <LhsTs, RhsTs>...>
    {
        return {
                std::pair{std::get<Indexes>(lhs), std::get<Indexes>(rhs)}...
        };
    }

    template <typename... LhsTs, typename... RhsTs>
    auto zip (const std::tuple<LhsTs...>& lhs, const std::tuple<RhsTs...>& rhs)
                                    -> std::tuple <std::pair <LhsTs, RhsTs>...>
    {
        return zip_impl(lhs, rhs, std::make_index_sequence<sizeof...(LhsTs)>{});
    }


}