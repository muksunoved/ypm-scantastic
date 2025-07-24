#pragma once

#include <expected>

#include "types.hpp"



namespace stdx::details {

constexpr size_t kMaxFormatStrSize = 256;
    
enum class spec_type {
    kSignedDigital,
    kUnsignedDigital,
    kString
};

template <same_as_char_type CharT>
struct valid_specs {
    static constexpr CharT data[] = {   static_cast<CharT>('d'), 
                                                    static_cast<CharT>('u'), 
                                                    static_cast<CharT>('s')};

    static consteval std::expected<spec_type, parse_error<>> get_spec_type(CharT symbol) {
        switch (symbol) {
            case static_cast<CharT>('d'):
            return spec_type::kSignedDigital;

            case static_cast<CharT>('u'):
            return spec_type::kUnsignedDigital;

            case static_cast<CharT>('s'):
            return spec_type::kString;

            default:
            return std::unexpected("Unknown specifier");
        }
    }
};


// Шаблонный класс для хранения форматирующей строчки и ее особенностей
// ваш код здесь
//template <typename CharT, size_t Len>
template <  same_as_char_type CharT, 
            basic_fixed_string<CharT, kMaxFormatStrSize> str  >
class basic_format_string {
    // ваш код здесь
public:

    static consteval std::expected<size_t, parse_error<>> get_number_placeholders() {
        constexpr size_t N = str_.size();
        if (!N)
            return 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str_.data[pos] != static_cast<CharT>('{')) {
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
            if (str_.data[pos] == static_cast<CharT>('%')) {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const CharT spec = str_.data[pos];
                bool valid = false;

                for (const CharT s : valid_specs<CharT>::data) {
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
            if (pos >= size || str_.data[pos] != static_cast<CharT>('}')) {
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
    
    using placeholders_pair_t = std::pair<size_t, size_t>;
    using placeholders_positions_t = std::array<placeholders_pair_t, number_placeholders_.value()>;

private:
    
    static constexpr placeholders_positions_t get_placeholders_positions() {
        size_t pos = 0;
        placeholders_positions_t result;
        for (size_t i = 0; i < number_placeholders_.value(); i++) {
            placeholders_pair_t p;
            size_t pos_first;
            size_t pos_second;
            while(str_.data[pos] != static_cast<CharT>('{')) {
                pos++;
            };
            pos_first = pos;
            while(str_.data[pos] != static_cast<CharT>('}')) {
                pos++;
            };
            pos_second = pos;
            result[i] = {pos_first, pos_second};
        }
        return result;
    } 
public:

    static constexpr placeholders_positions_t placeholders_positions_ = get_placeholders_positions();

};

template <fixed_string<kMaxFormatStrSize> str>
using format_string  = basic_format_string<char, str>;

template <wfixed_string<kMaxFormatStrSize> str>
using wformat_string  = basic_format_string<wchar_t, str>;


// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/

template <fixed_string<kMaxFormatStrSize> str>
constexpr auto operator"" _fs() {
    return format_string<str>();
}

template <wfixed_string<kMaxFormatStrSize> str>
constexpr auto operator"" _wfs() {
    return wformat_string<str>();
}

} // namespace stdx::details
