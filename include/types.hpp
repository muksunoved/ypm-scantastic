#pragma once

#include <cstddef>
#include <algorithm>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

// ваш код здесь
template <size_t Len = 0>
struct fixed_string {
    // ваш код здесь
    char f_string[Len] = {};

    constexpr fixed_string() = default;

    constexpr fixed_string(const char (&c)[Len]) {
        for (size_t i = 0; i<Len-1 && c[i] != 0; i++) {
            f_string[i] = c[i];
        }
    }
    template <size_t OtherLen>
    constexpr fixed_string(const char  (&c)[OtherLen]) requires requires { OtherLen <= Len; } {
        for (size_t i = 0; i<OtherLen; i++) {
            f_string[i] = c[i];
        }
    }
    constexpr fixed_string(const char *c1, const char *c2) {
        size_t i = 0;
        size_t j = 0;

        for (i = i; i<Len-1 && c1[i] != 0; i++) {
            f_string[i] = c1[i];
        }
        for (j = i; j<Len-1 && c2[j] != 0; j++) {
            f_string[j] = c2[j];
        }
    }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : public fixed_string<64> {
    fixed_string err{"No error"};
};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
// ваш код здесь
// измените реализацию
    int i;
};

} // namespace stdx::details
