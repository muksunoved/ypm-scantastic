#include <print>
#include "scan.hpp"
    

using namespace stdx::details;

    
constexpr wchar_t wtest_string_1[] =  {L"Test error string 1"};
constexpr char    test_string_1 [] =  { "Test error string 1"};

int main() { 

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

    //static_assert(stdx::scan<wformat_string<L"test">{}, stdx::details::wfixed_string{}, int>().values<0>() == 42); 


}
