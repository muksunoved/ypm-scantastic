#pragma once

#include <expected>

#include "types.hpp"



namespace stdx::details {

constexpr size_t kMaxFormatStrLen = 128;


// Шаблонный класс для хранения форматирующей строчки и ее особенностей
// ваш код здесь
//template <typename CharT, size_t Len>
template <fixed_string<default_char, kMaxFormatStrLen> str>
class format_string {
    // ваш код здесь
    using placeholders_t = std::array<std::pair<size_t, size_t>>;

private:
    static consteval placeholders_t get_placeholders_positions() {
        //TODO: last modufication
    } 

    static consteval std::expected<size_t, parse_error<>> get_number_placeholders() {
        constexpr size_t N = str_.size();
        if (!N)
            return 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str_.data[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (str_.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = str_.data[pos];
                constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || str_.data[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }
            ++pos;
        }
        return placeholder_count;
    }

public:
    static constexpr auto str_ = str;
    static constexpr auto number_placeholders_ = get_number_placeholders();
    static_assert(number_placeholders_, number_placeholders_.error().get_data() );
};

// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/

// Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
// Функция закомментирована, так как еще не реализованы классы, которые она использует
// Сделайте эту свободную функцию методом класса format_string

// Функция для получения позиций плейсхолдеров

// ваш код здесь
void get_placeholder_positions() {  // сигнатуру тоже нужно изменить
    // ваш код здесь
}

} // namespace stdx::details
