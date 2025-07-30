#pragma once

#include <charconv>
#include <concepts>
#include <optional>
#include <system_error>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

template <same_as_char_type CharT>
struct valid_digit_symbols {
    static constexpr std::array<std::pair<CharT, int>, 12> data  = {{   
                            { static_cast<CharT>(' '), -1 }, 
                            { static_cast<CharT>('-'), -1 }, 
                            { static_cast<CharT>('0'),  0}, 
                            { static_cast<CharT>('1'),  1}, 
                            { static_cast<CharT>('2'),  2}, 
                            { static_cast<CharT>('3'),  3}, 
                            { static_cast<CharT>('4'),  4}, 
                            { static_cast<CharT>('5'),  5}, 
                            { static_cast<CharT>('6'),  6}, 
                            { static_cast<CharT>('7'),  7}, 
                            { static_cast<CharT>('8'),  8}, 
                            { static_cast<CharT>('9'),  9}, 
    }};
    
    static constexpr size_t first_of_digitals = 2;
    static constexpr size_t minus_position = 1;
    static constexpr size_t space_position = 0;

    static consteval bool is_digital(CharT c) {
        for (auto d : data) {
            if (d.first == c)
                return true;
        }
        return false;
    }
    static consteval bool is_minus(CharT c) {
        return (c == data[minus_position].first);
    }
    static consteval bool is_space(CharT c) {
        return (c == data[space_position].first);
    }
    static consteval std::expected<int, parse_error<>> to_number(CharT c) {
        for (auto d : data) {
            if (d.first == c)
                return d.second;
        }

        return std::unexpected("Can't convert symbol");

    }
};

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
template<size_t I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    constexpr auto number_placeholders = fmt.number_placeholders_.value();
    static_assert(I >= 0 && I < number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto& fs) {
        return std::string_view(fs.data, fs.size() - 1);
    };

    constexpr auto fmt_sv = to_sv(fmt.str_);
    constexpr auto src_sv = to_sv(source);
    constexpr auto& positions = fmt.placeholders_positions_;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&]{
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I-1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I-1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&]{
        // Получаем разделитель после текущего плейсхолдера
        if constexpr(fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep = fmt_sv.substr(fmt_end + 1,
            (I < number_placeholders - 1)
                ? positions[I+1].first - (fmt_end + 1)
                : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}

// Семейство функция parse_value
template<supported_value_type ValueT>
consteval std::expected<ValueT, parse_error<>> parse_unsigned_value(const fixed_string<>& source, const size_t first, const size_t last, bool block_spaces = false) {
    if (!source.size()) {
        return std::unexpected("Invalid digital string size");
    }

    size_t local_first = first;
    size_t local_last = last;

    if (!block_spaces) {
        for (size_t pos = first; pos < last; pos++) {
            if (valid_digit_symbols<char>::is_space(source.data[pos])) {
                local_first++;
            } else {
                break;
            }
        }
    }
    for (size_t pos = last-1; pos > local_first; pos--) {
        if (valid_digit_symbols<char>::is_space(source.data[pos])) {
            local_last--;
        } else {
            break;
        }
    }

    for (size_t pos = local_first; pos < local_last; pos++) {
        if (!valid_digit_symbols<char>::is_digital(source.data[pos]))
            return std::unexpected("Invalid digital symbol");

    }

    ValueT value = 0;

    for (size_t pos = local_first; pos < local_last; pos++) {
        auto res = valid_digit_symbols<char>::to_number(source.data[pos]);
        
        if (!res) {
            return std::unexpected("Invalid digit symbol");
        };

        max_value_type_t digit = res.value();
        max_value_type_t limit  = std::numeric_limits<ValueT>::max();
        if (limit / 10  < value) {
            return std::unexpected("Value numeric limit overflow");
        } 
        value *= 10;
        if (limit - value < digit) {
            return std::unexpected("Value numeric limit overflow");
        }
        value += digit;
    }
    return value;
}

template<supported_value_type ValueT>
consteval std::expected<ValueT, parse_error<>> parse_signed_value(const fixed_string<>& source, const size_t first, const size_t last) {
    if (!source.size()) {
        return std::unexpected("Invalid digital string size");
    }
    int minus = 1;
    size_t shift = 0;
    if (valid_digit_symbols<char>::is_minus(source.data[first])) {
        minus = -1;
        shift = 1;
    }
    auto is_minus = minus < 0;
    
    auto res = parse_unsigned_value<max_value_type_t>(source, first+shift, last, (is_minus ? true : false) );

    if (!res) {
        return std::unexpected("Parse base unsigned value error");
    }

    max_value_type_t value = res.value();
    max_value_type_t limit_max  = std::numeric_limits<ValueT>::max();

    if (!is_minus) {
        if (limit_max < value) { 
            return std::unexpected("Value numeric limit overflow");
        }
    } else {
        max_value_type_t limit_min  = std::make_unsigned_t<ValueT>(std::numeric_limits<ValueT>::min()  ) ;
        if (limit_min < value) { 
            return std::unexpected("Value numeric limit overflow");
        }
    }
    return minus * value;
}

template<supported_value_type ValueT>
consteval std::expected<std::string_view, parse_error<>> parse_string_value(const fixed_string<>& source, const size_t first, const size_t last) {
    if (!source.size()) {
        return std::unexpected("Invalid digital string size");
    }
    return std::string_view(&source.data[first], last-first);
}

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера
template<size_t I, format_string fmt, fixed_string source, supported_value_type ValueT>
consteval std::expected<ValueT, parse_error<>> parse_input() {  // поменяйте сигнатуру
    auto [src_first, src_last] = get_current_source_for_parsing<I,fmt,source>();
    auto [f_p, s_p] = fmt.placeholders_positions_[I];

    for (size_t pos = f_p; pos < s_p; pos++) {
        if (fmt.str_.data[pos] == '%') {
            auto fv = valid_specs<char>::get_spec_type(fmt.str_.data[pos+1]);
            if (!fv)
                return std::unexpected("Can't get specifier type");

            if constexpr (signed_digital_value_type<ValueT>) {

                if  (fv == spec_type::kSignedDigital) 
                    return parse_signed_value<ValueT>(source, src_first, src_last);
            
            } else if constexpr (unsigned_digital_value_type<ValueT>) {

                if  (fv == spec_type::kUnsignedDigital) 
                    return parse_unsigned_value<ValueT>(source, src_first, src_last);

            } else if constexpr (string_value_type<ValueT>) {
                if  (fv == spec_type::kString) 
                    return parse_string_value<ValueT>(source, src_first, src_last);
            
            } else {
                return std::unexpected("Unsupported value type");
            }
        }
    }
    // update empty placeholder
    return std::unexpected("Unknown error");
}

} // namespace stdx::details
