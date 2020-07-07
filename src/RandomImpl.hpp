#pragma once

#include <random>
#include <algorithm>


namespace stq {

    namespace detail {
        struct NoDistrubutionTag{};
    }

    namespace impl {

        template<typename Integer, typename Dist>
        auto random_integer(Dist& dist) -> Integer
        {
            std::mt19937 gen{std::random_device{}()};

            if constexpr (std::is_same_v<Dist, detail::NoDistrubutionTag>) {
                return std::uniform_int_distribution<Integer>{}(gen);
            }
            else {
                return dist(gen);
            }
        }

        template <typename Floating, typename Dist>
        auto random_floating(Dist& dist) -> Floating
        {
            std::mt19937 gen{std::random_device{}()};

            if constexpr (std::is_same_v<Dist, detail::NoDistrubutionTag>) {
                return std::uniform_real_distribution<Floating>{}(gen);
            }
            else {
                return dist(gen);
            }
        }

        template<typename Dist>
        auto random_char(Dist& dist) -> char
        {

            if constexpr (std::is_same_v<Dist, detail::NoDistrubutionTag>) {
                auto temp_dist = std::uniform_int_distribution{32, 126};
                return random_integer<int>(temp_dist);
            }
            else {
               return random_integer<int>(dist);
            }
        }

        template <typename Dist>
        auto random_string(std::size_t size, Dist& dist) -> std::string
        {
            std::string str(size, '_');
            std::generate(std::begin(str), std::end(str), [&]
            {
                return random_char(dist);
            });
            return str;
        }

        template<typename Container, typename Gen, typename Dist>
        auto random_push_backable(std::size_t size, Dist& dist) -> Container
        {
            Container cont;
            if constexpr (detail::is_reservable <Container>) {
                cont.reserve(size);
            }

            for (auto i = 0; i < size; ++i) {
                cont.push_back(Gen{}.distribution(dist)
                                    .template generate<typename Container::value_type>());
            }
            return cont;
        }

        template<typename Container, typename Gen, typename Dist>
        auto random_insertable(std::size_t size, Dist& dist) -> Container
        {
            Container cont;
            for (auto i = 0; i < size; ++i) {
                cont.insert(Gen{}.distribution(dist)
                                 .template generate<typename Container::value_type>());
            }
            return cont;
        }

        template<typename Container, typename Gen, typename Dist>
        auto random_index_assignable(std::size_t size, Dist& dist) -> Container
        {
            Container cont;
            for (auto i = 0; i < size; ++i) {
                cont[i] = Gen{}.distribution(dist)
                               .template generate<typename Container::value_type>();
            }
            return cont;
        }


        template <typename Container, std::size_t Size, typename Dist, typename Gen>
        auto random_static_container (Dist& dist) -> Container
        {
            if constexpr (detail::has_push_back<Container>) {
                return random_push_backable<Container, Gen>(Size, dist);
            }
            else if constexpr (detail::has_insert<Container>) {
                return random_insertable<Container, Gen>(Size, dist);
            }
            else if constexpr (detail::is_random_index_assignable<Container>) {
                return random_index_assignable<Container, Gen>(Size, dist);
            }
            else {
                static_assert(detail::always_false <Container>, "Random is not implemented for this type");
            }
        }

        template<typename Container, typename Gen, typename Dist>
        auto random_dynamic_container(std::size_t size, Dist& dist) -> Container
        {
            if constexpr (std::is_same_v <Container, std::string>) {
                return random_string(size, dist);
            }
            else if constexpr (detail::has_push_back <Container>) {
                return random_push_backable<Container, Gen>(size, dist);
            }
            else if constexpr (detail::has_insert <Container>) {
                return random_insertable<Container, Gen>(size, dist);
            }
            else if constexpr (detail::is_random_index_assignable <Container>) {
                return random_index_assignable<Container, Gen>(size, dist);
            }
            else {
                static_assert(detail::always_false <Container>, "Random is not implemented for this type");
            }
        }
    }
}