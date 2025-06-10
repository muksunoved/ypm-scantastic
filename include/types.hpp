#pragma once

#include <cstddef>
#include <algorithm>
#include <tuple>
#include <type_traits>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template <typename CharT = char, size_t Len = 0>
requires  std::is_integral_v<CharT>
struct fixed_string {
    // ваш код здесь
    CharT f_string[Len] = {};

    constexpr fixed_string() = default;

    constexpr fixed_string(const CharT (&s)[Len]) noexcept {
        std::copy_n(s,Len,f_string);
    }
    template <size_t OtherLen>
    constexpr fixed_string(const CharT  (&s)[OtherLen]) noexcept requires 
    requires { 
        OtherLen <= Len; 
    }  {
        std::copy_n(s,OtherLen,f_string);
    }

    constexpr fixed_string(const CharT *begin, const CharT *end) noexcept {
        const CharT *ptr = begin;

        if (begin) {
            for (size_t i = 0; i<Len-1 && ptr[i] != 0 && &ptr[i] != end; i++) {
                f_string[i] = ptr[i];
            }
        }
    }

    constexpr size_t get_strlen() {
        size_t result = 0;
        while(f_string[result]) {
            ++result;
        }
        return result;
    }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
template <typename CharT, size_t Len = 128>
struct parse_error : public fixed_string<CharT, Len> {
    constexpr parse_error(const CharT* s) : fixed_string<CharT, Len>(s, nullptr) {} 
};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
// ваш код здесь
// измените реализацию
    constexpr scan_result(Ts &&...args) {
        values_ = std::make_tuple(args...);
    }
    std::tuple<Ts ... > values_{};

    template <size_t index>
    constexpr auto values() {
        return std::get<index>(values_);
    }
};

} // namespace stdx::details
