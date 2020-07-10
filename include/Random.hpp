#pragma once

#include "../src/Generator.hpp"
#include "../src/Schema.hpp"

namespace stq {

    template <typename T, typename Schema>
    auto random (Schema&& schema) -> T
    {
        return Generator<T>{}(std::forward<Schema>(schema));
    }

}