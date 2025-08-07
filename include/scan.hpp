#pragma once

#include <tuple>
#include <utility>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

template<std::size_t N, typename... Ts>
using elem_type = typename std::tuple_element<N, std::tuple<Ts...>>::type;

template < details::format_string fmt, details::fixed_string source, typename... Ts, size_t... Is>
constexpr details::scan_result<Ts...> scan_w_indexes(std::index_sequence<Is...> ) {
    using Tuple = typename std::tuple<std::remove_cv_t<Ts>...>;
    Tuple tp;        
    ((std::get<Is>(tp) = details::parse_input<Is, fmt, source, elem_type<Is, std::remove_cv_t<Ts>...>>().value()), ...);
    details::scan_result<Ts...>res{{tp}};
    return res;
}

template < details::format_string fmt, details::fixed_string source, typename... Ts>
requires details::all_supported<Ts...>
constexpr std::expected<details::scan_result<Ts...>, details::parse_error<>> scan() {
    if constexpr (sizeof... (Ts) == fmt.number_placeholders_) {
        return scan_w_indexes<fmt,source,Ts...>(std::make_index_sequence<sizeof... (Ts)>{});
    } 
    
    return std::unexpected("Mismatch plaseholders number and count of parameters list");
}

} // namespace stdx