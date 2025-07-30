#pragma once

#include <cstddef>
#include <algorithm>
#include <tuple>
#include <type_traits>

namespace stdx::details {

#define CharPrefix L

constexpr size_t kDfltFixedStringSize   = 256;
constexpr size_t kDfltErrorStrSize      = 256;
constexpr size_t kMaxFormatStrSize      = 256;


using max_value_type_t = uint64_t;

template <typename T>
concept same_as_char_type = (std::same_as<T, char> || std::same_as<T, wchar_t>);

template <typename T>
concept supported_value_type = (
                                       std::same_as<std::remove_cv_t<T>, int8_t> 
                                    || std::same_as<std::remove_cv_t<T>, int16_t>
                                    || std::same_as<std::remove_cv_t<T>, int32_t>
                                    || std::same_as<std::remove_cv_t<T>, int64_t>
                                    || std::same_as<std::remove_cv_t<T>, uint8_t>
                                    || std::same_as<std::remove_cv_t<T>, uint16_t>
                                    || std::same_as<std::remove_cv_t<T>, uint32_t>
                                    || std::same_as<std::remove_cv_t<T>, uint64_t>
                                    || std::same_as<std::remove_cv_t<T>, std::string_view>
                                );
template <typename T>
concept signed_digital_value_type = (
                                       std::same_as<std::remove_cv_t<T>, int8_t> 
                                    || std::same_as<std::remove_cv_t<T>, int16_t>
                                    || std::same_as<std::remove_cv_t<T>, int32_t>
                                    || std::same_as<std::remove_cv_t<T>, int64_t>
                                );
template <typename T>
concept unsigned_digital_value_type = (
                                       std::same_as<std::remove_cv_t<T>, uint8_t>
                                    || std::same_as<std::remove_cv_t<T>, uint16_t>
                                    || std::same_as<std::remove_cv_t<T>, uint32_t>
                                    || std::same_as<std::remove_cv_t<T>, uint64_t>
                                );
template <typename T>
concept string_value_type = (
                                     std::same_as<std::remove_cv_t<T>, std::string_view>
                                );

// Шаблонный класс, хранящий C-style строку фиксированной длины
template <same_as_char_type CharT, size_t Len = kDfltFixedStringSize>
struct basic_fixed_string {
    CharT data[Len+1] = {};

    consteval basic_fixed_string() = default;

    consteval basic_fixed_string(const CharT (&s)[Len]) noexcept {
        std::copy_n(s,Len,data);
    }
    template <size_t OtherLen>
    consteval basic_fixed_string(const CharT  (&s)[OtherLen]) noexcept requires 
    requires { 
        OtherLen <= Len; 
    }  {
        std::copy_n(s,OtherLen,data);
    }

    consteval basic_fixed_string(const CharT * begin, const CharT * end) noexcept {
        const CharT *ptr = begin;

        if (begin) {
            for (size_t i = 0; i<Len-1 && ptr[i] != 0 && &ptr[i] != end; i++) {
                data[i] = ptr[i];
            }
        }
    }

    consteval size_t size() const noexcept {
        size_t result = 0;
        while(data[result]) {
            ++result;
        }
        return ++result;
    }

    consteval std::basic_string_view<CharT> get_data() const noexcept {
        return std::basic_string_view<CharT>(data, size()-1);
    }
};

template <size_t Len = kDfltFixedStringSize>
using fixed_string  = basic_fixed_string<char, Len>;

template <size_t Len = kDfltFixedStringSize>
using wfixed_string = basic_fixed_string<wchar_t, Len>;



// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга
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
    consteval scan_result() = default ;
    
    consteval scan_result(Ts &&...args) {
        values_ = std::tuple<Ts...>(args...);
    }

    consteval scan_result(const std::tuple<Ts...>& tp) :
    values_(tp) {
    }
    
    std::tuple<Ts... > values_;

    template <size_t index>
    consteval auto& values() {
        return std::get<index>(values_);
    }
    
    template <size_t index>
    consteval auto values() const {
        return std::get<index>(values_);
    }
    
    template <size_t index>
    consteval std::tuple_element_t<index, std::tuple<Ts...>>&& values_for_t() const {
        return std::forward(std::get<index>(values_));
    }
};

} // namespace stdx::details
