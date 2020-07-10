#pragma once

#include "TypeTraits/GeneralTraits.hpp"
#include "TypeTraits/ContainerTraits.hpp"
#include "Utils.hpp"
#include "Schema.hpp"

#include <optional>
#include <random>
#include <functional>

namespace stq {

    template <typename T, typename Schema>
    auto random(Schema&& schema) -> T;

    template <typename T, typename = void>
    struct Generator {
        static_assert(detail::always_false<T>, "Random is not implemented for this type");
    };

    template<typename T>
    struct Generator <T, std::enable_if_t<std::is_integral_v<T>>> {

        template <typename Dist>
        auto operator()(Arithmetic<Dist> schema) -> T
        {
            std::mt19937 gen{std::random_device{}()};

            if constexpr (std::is_same_v<Dist, detail::NoDistribution>) {
                return std::uniform_int_distribution<T>{}(gen);
            }
            else {
                return schema.distribution()(gen);
            }
        }
    };

    template<typename T>
    struct Generator <T, std::enable_if_t<std::is_floating_point_v<T>>> {

        template <typename Dist>
        auto operator()(Arithmetic<Dist> schema) -> T
        {
            std::mt19937 gen{std::random_device{}()};

            if constexpr (std::is_same_v<Dist, detail::NoDistribution>) {
                return std::uniform_real_distribution<T>{}(gen);
            }
            else {
                return schema.distribution()(gen);
            }
        }
    };

    template <typename T>
    struct Generator<T, std::enable_if_t<detail::trait::is_smart_owning_pointer<T>>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> T
        {
            return T {new typename T::element_type {
                    random<typename T::element_type>(std::forward<Schema>(schema))
            }};
        }
    };

    template <typename T>
    struct Generator<T, std::enable_if_t<std::is_pointer_v<T>>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> T
        {
            return new std::remove_pointer_t<T> {
                random<std::remove_pointer_t<T>>(std::forward<Schema>(schema))
            };
        }
    };

    template <typename T>
    struct Generator<T, std::enable_if_t<detail::trait::is_container<T>>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> T
        {
            static_assert(detail::trait::is_template_instantiation<Schema, Container>);

            if constexpr (detail::trait::has_push_back <T>) {
                return random_push_backable<T>(std::forward<Schema>(schema));
            }
            else if constexpr (detail::trait::has_insert <T>) {
                return random_insertable<T>(std::forward<Schema>(schema));
            }
            else if constexpr (detail::trait::is_random_index_assignable <T>) {
                return random_index_assignable<T>(std::forward<Schema>(schema));
            }
            else {
                static_assert(detail::always_false <T>, "Random is not implemented for this type");
            }
        }

    private:

        template <typename Container, typename Schema>
        static auto random_push_backable(Schema&& schema) -> Container
        {
            Container cont;
            if constexpr (detail::trait::is_reservable <Container>) {
                cont.reserve(schema.size());
            }

            for (auto i = 0; i < schema.size(); ++i) {
                cont.push_back(random<typename Container::value_type>(schema.value()));
            }
            return cont;
        }

        template <typename Container, typename Schema>
        static auto random_insertable(Schema&& schema) -> Container
        {
            Container cont;
            for (auto i = 0; i < schema.size(); ++i) {
                cont.insert(random<typename Container::value_type>(schema.value()));
            }
            return cont;
        }

        template <typename Container, typename Schema>
        static auto random_index_assignable(Schema&& schema) -> Container
        {
            std::remove_const_t<Container> cont;
            if constexpr (detail::trait::is_resizable<Container>) {
                cont.resize(schema.size());
            }

            for (auto i = 0; i < schema.size(); ++i) {
                cont[i] = random<typename Container::value_type>(schema.value());
            }
            return cont;
        }
    };

    template <typename T, typename U>
    struct Generator<std::pair<T, U>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> std::pair<T, U>
        {
            static_assert(detail::trait::is_template_instantiation<Schema, Compound>,
                          "Schema for std::pair must be Compound");
            static_assert(std::tuple_size_v<std::decay_t<decltype(schema.values())>> == 2,
                          "Mismatched amount of nested types in compound type");
            return std::pair{
                random<T>(std::get<0>(schema.values())),
                random<U>(std::get<1>(schema.values()))
            };
        }
    };

    template <typename... Ts>
    struct Generator <std::tuple<Ts...>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> std::tuple<Ts...>
        {
            static_assert(detail::trait::is_template_instantiation<Schema, Compound>,
                          "Schema for std::tuple must be Compound");

            static_assert(sizeof...(Ts) == std::tuple_size_v<std::decay_t<decltype(schema.values())>>,
                          "Mismatched amount of nested types in compound type");

            return std::apply([](auto&&... tuple_elements) {
                return std::tuple{
                    random<Ts>(tuple_elements)...
                };
            }, schema.values());
        }
    };

    template <typename... Ts>
    struct Generator <std::variant<Ts...>> {

        template <typename Schema>
        auto operator() (Schema&& schema) -> std::variant<Ts...>
        {
            const auto choice = random<std::size_t> (
                    Arithmetic{std::uniform_int_distribution{std::size_t{0}, sizeof...(Ts)}}
            );

            const auto types = std::tuple {TypeWrapper<Ts>{}...};
            auto types_and_schemas = detail::zip(types, schema.values());

            std::variant <Ts...> variant;

            const auto assign_random = [&] (const auto& type_and_schema) {
                variant = random <typename std::decay_t<decltype(type_and_schema.first)>::type> (type_and_schema.second);
            };

            invoke_with_nth_elem<decltype(assign_random), 0>(assign_random, types_and_schemas, choice);
            return variant;
        }

    private:
        template <typename Func, std::size_t I, typename... Args>
        static void invoke_with_nth_elem (const Func& func, const std::tuple<Args...>& tuple, std::size_t n) {

            if (n == I) {
                func(std::get<I>(tuple));
            }
            if constexpr (I + 1 < sizeof...(Args)) {
                invoke_with_nth_elem<Func, I + 1, Args...>(func, tuple, n);
            }
        }

        template <typename T>
        struct TypeWrapper {
            using type = T;
        };
    };

}