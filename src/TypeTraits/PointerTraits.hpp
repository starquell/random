#pragma once

#include <memory>

namespace stq::detail::trait {

    namespace smart_ptr {

        template <typename T, typename = void>
        inline constexpr bool has_nested_element = false;

        template <typename T>
        inline constexpr bool has_nested_element <T, std::void_t<typename T::element_type>> = true;

        template <typename T, typename = void>
        inline constexpr bool has_dereferencing = false;

        template <typename T>
        inline constexpr bool has_dereferencing<T, std::enable_if_t<
                has_nested_element<T> &&
                std::is_same_v<decltype(*std::declval<T>()), typename T::element_type&>
        >> = true;
    }

    template <typename T, typename = void >
    inline constexpr bool is_smart_owning_pointer = false;

    template <typename T>
    inline constexpr bool is_smart_owning_pointer <T, std::enable_if_t<
                            smart_ptr::has_dereferencing<T> &&
                            std::is_constructible_v<T, typename T::element_type*>>> = true;
}