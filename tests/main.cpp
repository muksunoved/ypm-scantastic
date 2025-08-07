#include <print>
#include "scan.hpp"
    

using namespace stdx::details;

    

int main() { 

    constexpr wchar_t wtest_string_1[] =  {L"Test error string 1"};
    constexpr char    test_string_1 [] =  { "Test error string 1"};

    // Fixed string last zero 
    static_assert(fixed_string<>(test_string_1).data[20] == 0, "Error implementation of fixed_string type");

    static_assert(wfixed_string<>(wtest_string_1).data[20] == 0, "Error implementation of fixed_string type");
    
    // Fixed string contain
    static_assert(fixed_string<>(test_string_1).get_data() == std::basic_string_view<char>(test_string_1), 
                            "Error implementation of fixed_string type");

    static_assert(wfixed_string<>(wtest_string_1).get_data() == std::basic_string_view<wchar_t>(wtest_string_1), 
                            "Error implementation of fixed_string type");

    // Fixed string contain inplace creation
    constexpr wchar_t wtest_2[] =  {L"Test error string 2"};
    static_assert(wfixed_string<sizeof(wtest_2) + 10>(wtest_2).size() == 20, "Error implementation of fixed_string type");

    static_assert(parse_error("Test error string 1").size() == 20, "Error implementation of parce_error type");
    

    static_assert(scan_result(1, 'c', 10.1).values<0>() == 1);
    static_assert(scan_result(1, 'c', 10.1).values<1>() == 'c');
    static_assert(scan_result(1, 'c', 10.1).values<2>() == 10.1);

    // Format string placeholders number
    static_assert(wformat_string<L"test">::number_placeholders_.value() == 0);
    static_assert(wformat_string<L"test {%u}">::number_placeholders_.value() == 1);
    static_assert(wformat_string<L"test {%d} test {%u}">::number_placeholders_.value() == 2);

    // Format string placeholders positions
    using test_fs_2_t = format_string<"test {%u} {%d} {%s}">;
    static_assert(test_fs_2_t::placeholders_positions_.size() == 3);
    static_assert(test_fs_2_t::placeholders_positions_ == test_fs_2_t::placeholders_positions_t{std::pair<size_t,size_t>{ 5, 8}, 
                                                                                                std::pair<size_t,size_t>{10,13},
                                                                                                std::pair<size_t,size_t>{15,18}} );

    // Fixed string literal
    
    constexpr auto test_literal_1 = "test_literal {%u} {%d} {%s} {%u}"_fs;
    static_assert(test_literal_1.placeholders_positions_.size() == 4);
    static_assert(test_literal_1.placeholders_positions_[0] == std::pair<size_t,size_t>{13,16}); 
    static_assert(test_literal_1.placeholders_positions_[1] == std::pair<size_t,size_t>{18,21}); 
    static_assert(test_literal_1.placeholders_positions_[2] == std::pair<size_t,size_t>{23,26}); 
    static_assert(test_literal_1.placeholders_positions_[3] == std::pair<size_t,size_t>{28,31}); 
    
    constexpr auto test_wliteral_1 = L"test_literal {%u} {%d} {%s} {%u}"_wfs;
    static_assert(test_wliteral_1.placeholders_positions_.size() == 4);
    static_assert(test_wliteral_1.placeholders_positions_[0] == std::pair<size_t,size_t>{13,16}); 
    static_assert(test_wliteral_1.placeholders_positions_[1] == std::pair<size_t,size_t>{18,21}); 
    static_assert(test_wliteral_1.placeholders_positions_[2] == std::pair<size_t,size_t>{23,26}); 
    static_assert(test_wliteral_1.placeholders_positions_[3] == std::pair<size_t,size_t>{28,31}); 

    static_assert(stdx::details::supported_value_type<uint8_t> == true);
    static_assert(stdx::details::supported_value_type<uint16_t> == true);
    static_assert(stdx::details::supported_value_type<uint32_t> == true);
    static_assert(stdx::details::supported_value_type<uint8_t*> == false);
    static_assert(stdx::details::supported_value_type<uint8_t&> == false);
    static_assert(stdx::details::supported_value_type<std::string_view> == true);
    static_assert(stdx::details::supported_value_type<std::string&> == false);
    static_assert(stdx::details::supported_value_type<std::string_view&> == false);
    static_assert(stdx::details::supported_value_type<float> == false);
    static_assert(stdx::details::supported_value_type<double> == false);


    constexpr auto test_parse_sv = parse_input<0, "test {%s} test"_fs, fixed_string<>("test test_string test"), std::string_view>();
    static_assert( test_parse_sv == std::string_view("test_string"));

    constexpr auto test_parse_d_1 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test 500 test"), int16_t>();
    static_assert( test_parse_d_1 == 500);

    constexpr auto test_parse_d_2 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test -500 test"), int16_t>();
    static_assert( test_parse_d_2 == -500);
    
    constexpr auto test_parse_d_3 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test 32767 test"), int16_t>();
    static_assert( test_parse_d_3 == 32767);
    
    constexpr auto test_parse_d_4 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test -32768 test"), int16_t>();
    static_assert( test_parse_d_4 == -32768);
    
    constexpr auto test_parse_d_5 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test 32768 test"), int16_t>();
    static_assert( !test_parse_d_5.has_value());
    
    constexpr auto test_parse_d_6 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test -32769 test"), int16_t>();
    static_assert( !test_parse_d_6.has_value());
    
    constexpr auto test_parse_d_7 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test 9223372036854775807 test"), int64_t>();
    static_assert( test_parse_d_7 == 9223372036854775807);
    
    constexpr auto test_parse_d_8 = parse_input<0, "test {%d} test"_fs, fixed_string<>("test -9223372036854775808 test"), int64_t>();
    static_assert( test_parse_d_8 == -9223372036854775808);

    constexpr auto test_parse_d_9 = parse_input<0, "test {%u} test"_fs, fixed_string<>("test 18446744073709551615 test"), uint64_t>();
    static_assert( test_parse_d_9 == 18446744073709551615);
    
    constexpr auto test_parse_d_10 = parse_input<0, "test {%u} test"_fs, fixed_string<>("test 18446744073709551616 test"), uint64_t>();
    static_assert( !test_parse_d_10.has_value());
    
    constexpr auto scan_res_1 = stdx::scan<"test {%d} test"_fs, fixed_string<>("test 32767 test"), int16_t>();
    static_assert( scan_res_1.value().values<0>() == 32767);

    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%u} test"_fs, fixed_string<>("test 32767 100000 test"), int16_t, uint32_t>();
    static_assert( scan_res_2.value().values<0>() == 32767);
    static_assert( scan_res_2.value().values<1>() == 100000);

    constexpr auto scan_res_3 
                    = stdx::scan<"test {%d} {%s} {%d} test"_fs, fixed_string<>("test 32767 test_string 127 test"), const int16_t, const std::string_view, int8_t>();
    static_assert( scan_res_3.value().values<0>() == 32767);
    static_assert( scan_res_3.value().values<1>() == std::string_view("test_string"));

    static_assert( std::is_same_v<decltype(scan_res_3.value().values_for_t<0>()), const int16_t&&>);
    static_assert( std::is_same_v<decltype(scan_res_3.value().values_for_t<1>()), const std::string_view&&>);
    static_assert( std::is_same_v<decltype(scan_res_3.value().values_for_t<2>()), int8_t&&>);
}
