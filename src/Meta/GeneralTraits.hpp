#pragma once

#include "ContainerTraits.hpp"
#include "PointerTraits.hpp"


namespace strq::detail::trait {

    template <typename T, template <typename...> typename Template>
    inline constexpr bool is_template_instantiation = false;

    template <typename... Params, template <typename...> typename Template>
    inline constexpr bool is_template_instantiation <Template <Params...>, Template> = true;

    template <typename... Params, template <typename...> typename Template>
    inline constexpr bool is_template_instantiation <const Template <Params...>, Template> = true;

    template <typename... Params, template <typename...> typename Template>
    inline constexpr bool is_template_instantiation <const Template <Params...>&, Template> = true;

    template <typename... Params, template <typename...> typename Template>
    inline constexpr bool is_template_instantiation <Template <Params...>&, Template> = true;

    template <typename T, template <typename...> typename... Templates>
    inline constexpr bool is_some_template_instantiation_of = (is_template_instantiation <T, Templates> || ...);

}