#pragma once

#include "DefaultSchema.hpp"
#include "Schema.hpp"
#include "TypeTraits/GeneralTraits.hpp"

namespace stq::detail {

    template <typename DefaultSchema, typename UserSchema>
    constexpr auto normalized_impl (DefaultSchema&& default_schema, UserSchema&& user_schema) {

        if constexpr (detail::trait::is_template_instantiation<UserSchema, Arithmetic>) {

            static_assert (detail::trait::is_template_instantiation<DefaultSchema, Arithmetic>,
                          "Incorrect schema for this type");
            return user_schema;
        }
        else if constexpr (detail::trait::is_template_instantiation<UserSchema, Container>) {

            static_assert( detail::trait::is_template_instantiation<DefaultSchema, Container>,
                          "Incorrect schema for this type");
            return Container{
                    normalized_impl(default_schema.value(), user_schema.value()),
                    user_schema.size()
            };
        }
        else if constexpr (detail::trait::is_template_instantiation<UserSchema, Compound>) {

            static_assert(detail::trait::is_template_instantiation<DefaultSchema, Compound>,
                          "Incorrect schema for this type");
//            static_assert(std::tuple_size_v<decltype(default_schema.values())> ==
//                          std::tuple_size_v<decltype(user_schema.values())>,
//                          "Incorrect schema for this type");

            const auto ziped = zip(default_schema.values(), user_schema.values());

            return std::apply([] (auto&&... values) {
                return Compound {
                    normalized_impl(values.first, values.second)...
                };
            }, ziped);
        }
        else if constexpr (std::is_same_v<std::decay_t<UserSchema>, Default>) {
            return default_schema;
        }
        else {
            static_assert(always_false<DefaultSchema>, "Unreachable");
        }
    }


    template <typename T, typename Schema>
    constexpr auto normalized (Schema&& schema) {

        return normalized_impl(make_schema<T>(), std::forward<Schema>(schema));
    }
}