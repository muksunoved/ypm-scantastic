#include "scan.hpp"

int main() { 
    constexpr char test_1[] =  {"Test error string 1"};
    static_assert(stdx::details::fixed_string<char, sizeof(test_1)>(test_1).get_strlen() == 19, "Error implementation of parce_error type");
    
    constexpr char test_2[] =  {"Test error string 2"};
    static_assert(stdx::details::fixed_string<char, sizeof(test_2) + 10>(test_2).get_strlen() == 19, "Error implementation of parce_error type");

    static_assert(stdx::details::parse_error("Test error string 1").get_strlen() == 19, "Error implementation of parce_error type");
    

    static_assert(stdx::details::scan_result(1, 'c', 10.1).values<0>() == 1);
    static_assert(stdx::details::scan_result(1, 'c', 10.1).values<1>() == 'c');
    static_assert(stdx::details::scan_result(1, 'c', 10.1).values<2>() == 10.1);

    static_assert(stdx::scan<stdx::details::format_string{}, stdx::details::fixed_string{}, int>().values<0>() == 42); 

}
