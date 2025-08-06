#include <gtest/gtest.h>
#include "scan.hpp"
    

using namespace stdx::details;

TEST(ScanTest, ShouldConvertSignedTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%d} {%d} {%d} test"_fs, 
                                        fixed_string<>("test -127 32767 -2147483648 -9223372036854775808 test"), 
                                        int8_t, int16_t, int32_t, int64_t>();

    EXPECT_EQ( scan_res_2.value().values<0>() , -127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 32767);
    EXPECT_EQ( scan_res_2.value().values<2>() , -2147483648);
    EXPECT_EQ( scan_res_2.value().values<3>() , -9223372036854775808);
}


