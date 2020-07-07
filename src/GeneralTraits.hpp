#pragma once

#include "ContainerTraits.hpp"


namespace stq::detail {

    template <typename...>
    inline constexpr auto always_false = false;
}