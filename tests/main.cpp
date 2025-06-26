#include "scan.hpp"
    
constexpr stdx::details::default_char test_1[] =  {"Test error string 1"};

int main() { 
    using namespace stdx::details;

    static_assert(fixed_string<default_char, sizeof(test_1)>(test_1).size() == 20, "Error implementation of fixed_string type");
    static_assert(fixed_string<default_char, sizeof(test_1)>(test_1).data[20] == 0, "Error implementation of fixed_string type");
    
    static_assert(fixed_string<default_char, sizeof(test_1)>(test_1).data[0] == 'T', "Error implementation of fixed_string type");
    
    //static_assert(fixed_string<default_char, sizeof(test_1)>(test_1) == "test", "Error implementation of fixed_string type");
    static_assert(fixed_string<default_char, sizeof(test_1)>(test_1).data[18] == '1', "Error implementation of fixed_string type");
    
    constexpr default_char test_2[] =  {"Test error string 2"};
    static_assert(fixed_string<default_char, sizeof(test_2) + 10>(test_2).size() == 20, "Error implementation of fixed_string type");

    static_assert(parse_error("Test error string 1").size() == 20, "Error implementation of parce_error type");
    

    static_assert(scan_result(1, 'c', 10.1).values<0>() == 1);
    static_assert(scan_result(1, 'c', 10.1).values<1>() == 'c');
    static_assert(scan_result(1, 'c', 10.1).values<2>() == 10.1);

    static_assert(format_string<"test">().get_number_placeholders() == 0);
    static_assert(format_string<"test {%u}">().get_number_placeholders() == 1);
    static_assert(format_string<"test {%d} test {%u}">().get_number_placeholders() == 2);


    //constexpr auto result = format_string<"test {%d} test {%u">().get_number_placeholders();
    //static_assert(result == parse_error{"Unclosed last placeholder"});

    static_assert(stdx::scan<format_string<"test">{}, stdx::details::fixed_string{}, int>().values<0>() == 42); 

}
