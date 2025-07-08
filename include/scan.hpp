#pragma once

#include <tuple>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

// // Главная функция
// template <  details::same_as_char_type CharT,
//             details::basic_format_string<   CharT,  
//                                             details::basic_fixed_string<CharT, 
//                                             details::kMaxFormatStrSize> str > fmt, 
//             details::basic_fixed_string<CharT, details::kDfltFixedStringSize> source, 
//             typename... Ts>
// struct basic_scan {
//     consteval details::scan_result<Ts...> operator()() { // передайте пакет параметров в scan_result
// // измените реализацию
//     return details::scan_result<Ts...>{42};
// }

// }

// template <  details::format_string str > fmt, 
//             details::fixed_string source, 
//             typename... Ts>
// using scan = basic_scan<wcahr_, fmt, source, Ts... > ;

} // namespace stdx