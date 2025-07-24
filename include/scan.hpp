#pragma once

#include <tuple>
#include <utility>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

template < details::format_string fmt, details::fixed_string source, typename... Ts, size_t I>
constexpr void scan_w_index(details::scan_result<Ts...>* res) {

        using selected_t = std::tuple_element_t<I, std::tuple<Ts...>>;
        res-> template values<I>() = details::parse_input<I, fmt, source, selected_t>();
}

template < details::format_string fmt, details::fixed_string source, typename... Ts, size_t... Is>
constexpr details::scan_result<Ts...> scan_w_indexes(std::index_sequence<Is...> ) {
        
    details::scan_result<Ts...> res{};

    ((res. template values<Is>() = details::parse_input<Is, fmt, source, std::tuple_element_t<Is, std::tuple<Ts...>>>().value()), ...);

    return res;
}

template < details::format_string fmt, details::fixed_string source, typename... Ts>
constexpr std::expected<details::scan_result<Ts...>, details::parse_error<>> scan() {
    if constexpr (sizeof... (Ts) == fmt.number_placeholders_) {

        return scan_w_indexes<fmt,source,Ts...>(std::make_index_sequence<sizeof... (Ts)>{});
    } 
    
    return std::unexpected("Mismatch plaseholders number and count of parameters list");
}

} // namespace stdx