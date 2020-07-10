#pragma once

#include <type_traits>

namespace stq::detail::trait {

    template <typename T, typename = void>
    inline constexpr bool has_push_back = false;

    template <typename T>
    inline constexpr auto has_push_back<T, std::void_t<
                    decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))
    >> = true;

    template <typename T, typename = void>
    inline constexpr bool has_insert = false;

    template <typename T>
    inline constexpr auto has_insert<T, std::void_t<
                decltype(std::declval<T>().insert(std::declval<typename T::value_type>()))
    >> = true;

    template <typename Container, typename = void>
    inline constexpr bool is_random_index_assignable = false;

    template <typename Container>
    inline constexpr bool is_random_index_assignable<Container, std::void_t<
            std::is_same<decltype(std::declval<Container>().operator[](std::declval<typename Container::size_type>())),
                    typename Container::reference>>> = true;

    template<typename T, typename = void>
    inline constexpr bool is_reservable = false;

    template<typename T>
    inline constexpr bool is_reservable<T, std::void_t<
            decltype(std::declval<T>().reserve(std::declval<typename T::size_type>()))
    >> = true;

    template<typename T, typename = void>
    inline constexpr bool is_resizable = false;

    template<typename T>
    inline constexpr bool is_resizable<T, std::void_t<
            decltype(std::declval<T>().resize(std::declval<typename T::size_type>()))
    >> = true;

    template <typename T, typename = void>
    inline constexpr bool is_container = std::is_array_v<T>;

    template <typename T, typename = void>
    inline constexpr bool is_static_container = false;

    template <template <typename, std::size_t> typename T, typename ValueType, std::size_t N>
    inline constexpr bool is_static_container <T<ValueType, N>> = is_container<T<ValueType, N>>;


    template <typename T>
    inline constexpr bool is_container <T,
            std::void_t<
                    typename T::value_type,
                    decltype(std::declval<T>().begin()),
                    decltype(std::declval<T>().end())>
    > = true;


    template <typename T, std::size_t N>
    inline constexpr bool is_static_container <T[N]> = true;

    template <typename T>
    inline constexpr bool size_of_static_container = false;

    template <template <typename, std::size_t> typename T, typename ValueType, std::size_t N>
    inline constexpr std::size_t size_of_static_container <T<ValueType, N>> = N;

    template <typename T, std::size_t N>
    inline constexpr std::size_t size_of_static_container <T[N]> = N;

    template <typename T>
    inline constexpr bool is_dynamic_container = is_container<T> && !is_static_container<T>;

    template <typename T, typename = void>
    inline constexpr bool is_container_adapter = false;

    template <typename T>
    inline constexpr bool is_container_adapter<T, std::void_t<
                    typename T::container_type,
                    std::enable_if_t<is_container<typename T::container_type>
                    >>> = true;

};

