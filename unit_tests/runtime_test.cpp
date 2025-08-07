#include <gtest/gtest.h>
#include <print>
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

TEST(ScanTest, ShouldConvertSignedConstTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%d} {%d} {%d} test"_fs, 
                                        fixed_string<>("test -127 32767 -2147483648 -9223372036854775808 test"), 
                                        const int8_t, const int16_t, const int32_t, const int64_t>();

    EXPECT_EQ( scan_res_2.value().values<0>() , -127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 32767);
    EXPECT_EQ( scan_res_2.value().values<2>() , -2147483648);
    EXPECT_EQ( scan_res_2.value().values<3>() , -9223372036854775808);

    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<0>()), const  int8_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<1>()), const int16_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<2>()), const int32_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<3>()), const int64_t&& >::value), true);
}

TEST(ScanTest, ShouldConvertMixedSignedConstTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%d} {%d} {%d} test"_fs, 
                                        fixed_string<>("test -127 32767 -2147483648 -9223372036854775808 test"), 
                                        const int8_t, int16_t, const int32_t, int64_t>();

    EXPECT_EQ( scan_res_2.value().values<0>() , -127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 32767);
    EXPECT_EQ( scan_res_2.value().values<2>() , -2147483648);
    EXPECT_EQ( scan_res_2.value().values<3>() , -9223372036854775808);

    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<0>()), const  int8_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<1>()),       int16_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<2>()), const int32_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<3>()),       int64_t&& >::value), true);
}

TEST(ScanTest, ShouldConvertUnsignedConstTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%u} {%u} {%u} {%u} test"_fs, 
                                        fixed_string<>("test 127 32767 2147483648 9223372036854775808 test"), 
                                        const uint8_t, const uint16_t, const uint32_t, const uint64_t>();

    EXPECT_EQ( scan_res_2.value().values<0>() , 127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 32767);
    EXPECT_EQ( scan_res_2.value().values<2>() , 2147483648);
    EXPECT_EQ( scan_res_2.value().values<3>() , 9223372036854775808);

    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<0>()), const  uint8_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<1>()), const uint16_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<2>()), const uint32_t&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<3>()), const uint64_t&& >::value), true);
}

TEST(ScanTest, ShouldConverToConstTypeStringValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%s} {%s} {%s} {%s} test"_fs, 
                                        fixed_string<>("test 127 32767 2147483648 9223372036854775808 test"), 
                                        const std::string_view, const std::string_view, const std::string_view, const std::string_view>();

    EXPECT_EQ( scan_res_2.value().values<0>() , "127");
    EXPECT_EQ( scan_res_2.value().values<1>() , "32767");
    EXPECT_EQ( scan_res_2.value().values<2>() , "2147483648");
    EXPECT_EQ( scan_res_2.value().values<3>() , "9223372036854775808");

    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<0>()), const std::string_view&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<1>()), const std::string_view&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<2>()), const std::string_view&& >::value), true);
    EXPECT_EQ( (std::is_same<decltype(scan_res_2.value().values_for_t<3>()), const std::string_view&& >::value), true);
}

TEST(ScanTest, ShouldConvertSignedTypeValuesWithConvertibleTypes) {
    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%d} {%d} {%d} test"_fs, 
                                        fixed_string<>("test -127 32767 -2147483648 -9223372036854775808 test"), 
                                        char, short, int, long long int>();

    EXPECT_EQ( scan_res_2.value().values<0>() , -127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 32767);
    EXPECT_EQ( scan_res_2.value().values<2>() , -2147483648);
    EXPECT_EQ( scan_res_2.value().values<3>() , -9223372036854775808);
}

TEST(ScanTest, ShouldConvertUnsignedTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%u} {%u} {%u} {%u} test"_fs, 
                                        fixed_string<>("test 255 65535 2147483647 9223372036854775807 test"), 
                                        uint8_t, uint16_t, uint32_t, uint64_t>();

    EXPECT_EQ( scan_res_2.value().values<0>() , 255);
    EXPECT_EQ( scan_res_2.value().values<1>() , 65535);
    EXPECT_EQ( scan_res_2.value().values<2>() , 2147483647);
    EXPECT_EQ( scan_res_2.value().values<3>() , 9223372036854775807);
}

TEST(ScanTest, ShouldConvertUnsignedTypeValuesWithConvertibleTypes) {
    constexpr auto scan_res_2 = stdx::scan<"test {%u} {%u} {%u} {%u} test"_fs, 
                                        fixed_string<>("test 255 65535 2147483647 9223372036854775807 test"), 
                                        unsigned char, unsigned short, unsigned int, unsigned long long int>();

    EXPECT_EQ( scan_res_2.value().values<0>() , 255);
    EXPECT_EQ( scan_res_2.value().values<1>() , 65535);
    EXPECT_EQ( scan_res_2.value().values<2>() , 2147483647);
    EXPECT_EQ( scan_res_2.value().values<3>() , 9223372036854775807);
}

TEST(ScanTest, ShouldConvertToStringTypeValues) {
    constexpr auto scan_res_2 = stdx::scan<"test {%s} {%s} {%s} {%s} test"_fs, 
                                        fixed_string<>("test 255 65535 2147483647 9223372036854775807 test"), 
                                        std::string_view, std::string_view, std::string_view, std::string_view>();

    EXPECT_EQ( scan_res_2.value().values<0>() , "255");
    EXPECT_EQ( scan_res_2.value().values<1>() , "65535");
    EXPECT_EQ( scan_res_2.value().values<2>() , "2147483647");
    EXPECT_EQ( scan_res_2.value().values<3>() , "9223372036854775807");
}

TEST(ScanTest, ShouldConvertToMixed) {
    constexpr auto scan_res_2 = stdx::scan<"test {%d} {%u} {%s} test"_fs, 
                                        fixed_string<>("test 127 65535 2147483647 test"), 
                                        unsigned short, uint32_t, std::string_view>();

    EXPECT_EQ( scan_res_2.value().values<0>() , 127);
    EXPECT_EQ( scan_res_2.value().values<1>() , 65535);
    EXPECT_EQ( scan_res_2.value().values<2>() , "2147483647");
}


