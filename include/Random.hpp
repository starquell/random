#pragma once

#include "../src/Generator.hpp"
#include "../src/Schema.hpp"
#include "../src/DefaultSchema.hpp"
#include "../src/NormalizedSchema.hpp"

namespace strq {

    template <typename T, typename Schema>
    auto random(Schema&& schema) -> T
    {
        return Generator<T>{}(detail::normalized<T>(std::forward<Schema>(schema)));
    }

    template <typename T>
    auto random() -> T
    {
        return Generator<T>{}(detail::make_schema<T>());
    }

}