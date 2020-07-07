#pragma once

#include "GeneralTraits.hpp"
#include "RandomImpl.hpp"

#include <optional>

namespace stq {

    template <typename Dist = detail::NoDistrubutionTag>
    class Generator {

    public:

        auto size (std::size_t size) -> Generator<Dist>&
        {
            _size = size;
            return *this;
        }

        template <typename NewDist>
        auto distribution (const NewDist& dist) -> Generator<NewDist>
        {
            Generator <NewDist> new_gen;
            new_gen._size = _size;
            new_gen._dist = dist;
            return new_gen;
        }

        template <typename T>
        auto generate() -> T
        {
            if constexpr (std::is_same_v<T, char>) {
                return impl::random_char(_dist);
            }
            else if constexpr (std::is_integral_v<T>) {
                return impl::random_integer<T>(_dist);
            }
            else if constexpr (std::is_floating_point_v<T>) {
                return impl::random_floating<T>(_dist);
            }
            else if constexpr (detail::is_static_container<T>) {
                return impl::random_static_container<T, detail::size_of_static_container<T>, Generator<Dist>>
                        (_dist);
            }
            else if constexpr (detail::is_dynamic_container<T>) {
                return impl::random_dynamic_container<T, Generator<Dist>>(_size.value_or(0), _dist);
            }
            else {
                static_assert(detail::always_false<T>, "Random is not implemented for this type");
            }
        }
    private:
        template <typename Dist_>
        friend class Generator;

        std::optional<std::size_t> _size;
        Dist _dist;
    };

}