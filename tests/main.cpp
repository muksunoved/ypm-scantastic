#include <print>
#include "scan.hpp"
    

using namespace stdx::details;

    
constexpr stdx::details::default_char test_string_1[] =  {"Test error string 1"};

int main() { 


    constexpr auto test_size_1 = fixed_string<default_char, sizeof(test_string_1)>(test_string_1).size();
    static_assert(test_size_1 == 20, "Error implementation of fixed_string type");


    // Last zero 
    static_assert(fixed_string<default_char, sizeof(test_string_1)>(test_string_1).data[20] == 0, "Error implementation of fixed_string type");
    
    
    static_assert(fixed_string<default_char, sizeof(test_string_1)>(test_string_1).get_data() == std::string_view(test_string_1), 
                            "Error implementation of fixed_string type");

    
    constexpr default_char test_2[] =  {"Test error string 2"};
    static_assert(fixed_string<default_char, sizeof(test_2) + 10>(test_2).size() == 20, "Error implementation of fixed_string type");

    static_assert(parse_error("Test error string 1").size() == 20, "Error implementation of parce_error type");
    

    static_assert(scan_result(1, 'c', 10.1).values<0>() == 1);
    static_assert(scan_result(1, 'c', 10.1).values<1>() == 'c');
    static_assert(scan_result(1, 'c', 10.1).values<2>() == 10.1);

    static_assert(format_string<"test">::number_placeholders_.value() == 0);
    static_assert(format_string<"test {%u}">::number_placeholders_.value() == 1);
    static_assert(format_string<"test {%d} test {%u}">::number_placeholders_.value() == 2);


    //constexpr auto result = format_string<"test {%d} test {%u">().get_number_placeholders();
    //static_assert(result == parse_error{"Unclosed last placeholder"});

    static_assert(stdx::scan<format_string<"test">{}, stdx::details::fixed_string{}, int>().values<0>() == 42); 

    const auto v = fixed_string<default_char, sizeof(test_string_1)>(test_string_1).get_data();
    std::println("{}", v);

}
