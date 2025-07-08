#pragma once

#include <cstddef>
#include <algorithm>
#include <tuple>
#include <type_traits>

namespace stdx::details {

#define CharPrefix L

constexpr size_t kDfltFixedStringSize = 256;
constexpr size_t kDfltErrorStrSize = 256;

template <typename T>
concept same_as_char_type = (std::same_as<T, char> || std::same_as<T, wchar_t>);

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template <same_as_char_type CharT, size_t Len = kDfltFixedStringSize>
struct basic_fixed_string {
    // ваш код здесь
    CharT data[Len+1] = {};

    constexpr basic_fixed_string() = default;

    constexpr basic_fixed_string(const CharT (&s)[Len]) noexcept {
        std::copy_n(s,Len,data);
    }
    template <size_t OtherLen>
    constexpr basic_fixed_string(const CharT  (&s)[OtherLen]) noexcept requires 
    requires { 
        OtherLen <= Len; 
    }  {
        std::copy_n(s,OtherLen,data);
    }

    constexpr basic_fixed_string(const CharT * begin, const CharT * end) noexcept {
        const CharT *ptr = begin;

        if (begin) {
            for (size_t i = 0; i<Len-1 && ptr[i] != 0 && &ptr[i] != end; i++) {
                data[i] = ptr[i];
            }
        }
    }

    constexpr size_t size() const noexcept {
        size_t result = 0;
        while(data[result]) {
            ++result;
        }
        return ++result;
    }

    constexpr std::basic_string_view<CharT> get_data() const noexcept {
        return std::basic_string_view<CharT>(data, size()-1);
    }
};

template <size_t Len = kDfltFixedStringSize>
using fixed_string  = basic_fixed_string<char, Len>;

template <size_t Len = kDfltFixedStringSize>
using wfixed_string = basic_fixed_string<wchar_t, Len>;



// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
template <size_t Len = kDfltErrorStrSize>
struct parse_error : public fixed_string<Len> {
    constexpr parse_error(const char* s) : fixed_string<Len>(s, nullptr) {} 
    constexpr bool operator==(const parse_error& pe) const {
        return *this == pe;
    }
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
