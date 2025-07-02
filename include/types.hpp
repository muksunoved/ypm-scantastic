#pragma once

#include <cstddef>
#include <algorithm>
#include <tuple>
#include <type_traits>

namespace stdx::details {

using default_char = char;
constexpr size_t kDfltFixedStringSize = 256;
constexpr size_t kDfltErrorStrSize = 256;

template <typename T>
concept same_as_char_type = (std::same_as<T, char> || std::same_as<T, wchar_t>);

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template <same_as_char_type CharT = default_char, size_t Len = kDfltFixedStringSize>
struct fixed_string {
    // ваш код здесь
    CharT data[Len+1] = {};

    constexpr fixed_string() = default;

    constexpr fixed_string(const CharT (&s)[Len]) noexcept {
        std::copy_n(s,Len,data);
    }
    template <size_t OtherLen>
    constexpr fixed_string(const CharT  (&s)[OtherLen]) noexcept requires 
    requires { 
        OtherLen <= Len; 
    }  {
        std::copy_n(s,OtherLen,data);
    }

    constexpr fixed_string(const CharT * begin, const CharT * end) noexcept {
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

    constexpr std::string_view get_data() const noexcept {
        return std::string_view(data, size()-1);
    }

    // constexpr bool operator==(const fixed_string& other) const {
    //     constexpr size_t N = this->size();
    //     if (N != other.size()) {
    //         return false;
    //     }
    //     for (size_t i = 0; i<N; i++) {
    //         if (this->data[i] != other.data[i]) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    // constexpr bool operator==(const CharT* s) const {
    //     const CharT *ptr = s;
    //     constexpr size_t N = this->size();
    //     size_t i = 0;

    //     if (ptr) {
    //         for (i = 0; i<N && ptr[i] != 0 ; i++) {
    //             if (this->data[i] != ptr[i]) {
    //                 return false;
    //             }
    //         }
    //         if (i < N) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
template <same_as_char_type CharT = default_char, size_t Len = kDfltErrorStrSize>
struct parse_error : public fixed_string<CharT,Len> {
    constexpr parse_error(const CharT* s) : fixed_string<CharT, Len>(s, nullptr) {} 
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
