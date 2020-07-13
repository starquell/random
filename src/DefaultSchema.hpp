#pragma once

#include "Meta/GeneralTraits.hpp"
#include "Schema.hpp"

namespace strq::detail {

    static constexpr std::size_t DEFAULT_CONTAINER_SIZE = 1;

    template <typename T, typename = void>
    struct SchemaGenerator {
        static_assert(always_false<T>, "No default schema for this type");
    };

    template <typename T>
    constexpr auto make_schema() {
        return SchemaGenerator<T>{}();
    }

    template <typename T>
    struct SchemaGenerator <T, std::enable_if_t<std::is_arithmetic_v<T>>> {

        constexpr auto operator()() -> Arithmetic<NoDistribution>
        {
            return Arithmetic{};
        }
    };

    template <typename T>
    struct SchemaGenerator <T, std::enable_if_t<trait::is_container<T> ||
                                                trait::is_container_adapter<T>>> {

        constexpr auto operator()() {
            return Container {
                    make_schema<typename T::value_type>(),
                    DEFAULT_CONTAINER_SIZE
            };
        }
    };

    template<typename T>
    struct SchemaGenerator<T, std::enable_if_t<trait::is_smart_owning_pointer<T>>> {

        constexpr auto operator()() {
            return Compound{make_schema<typename T::element_type>()};
        }
    };

    template<typename T>
    struct SchemaGenerator<T, std::enable_if_t<std::is_pointer_v<T>>> {

        constexpr auto operator()() {
            return Compound{make_schema<std::remove_pointer_t<T>>()};
        }
    };

    template <typename... Ts>
    struct SchemaGenerator <std::tuple<Ts...>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<Ts>()...
            };
        }
    };

    template <typename... Ts>
    struct SchemaGenerator <std::variant<Ts...>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<Ts>()...
            };
        }
    };

    template <typename T, typename U>
    struct SchemaGenerator <std::pair<T, U>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<T>(),
                make_schema<U>()
            };
        }
    };

    template <typename T>
    struct SchemaGenerator <std::optional<T>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<T>()
            };
        }
    };

    template <typename T>
    struct SchemaGenerator <std::atomic<T>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<T>()
            };
        }
    };

    template <typename T>
    struct SchemaGenerator <std::complex<T>> {

        constexpr auto operator()() {
            return Compound {
                make_schema<T>(),
                make_schema<T>()
            };
        }
    };


}