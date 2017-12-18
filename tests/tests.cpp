#include "jsteemann/atoi.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <iostream>
#include <algorithm>

template<typename T>
static void test(bool shouldBeValid, std::string const& value) {
  bool valid;
  T result = jsteemann::atoi<T>(value.c_str(), value.size(), valid);
  ASSERT_EQ(shouldBeValid, valid);
  if (shouldBeValid) {
    ASSERT_EQ(value, std::to_string(result));
  }
}

template<typename T>
static void test(T expected, std::string const& value) {
  bool valid;
  T result = jsteemann::atoi<T>(value.c_str(), value.size(), valid);
  ASSERT_TRUE(valid);
  ASSERT_EQ(expected, result);
}

TEST(atoiTest, testStrangeNumbers) {
  test<int64_t>(int64_t(0), "00"); 
  test<int64_t>(int64_t(0), "00000000000000000000000000000"); 
  test<int64_t>(int64_t(1), "01"); 
  test<int64_t>(int64_t(0), "-0"); 
  test<int64_t>(int64_t(-1), "-01"); 
  test<int64_t>(int64_t(-10), "-010"); 
  test<int64_t>(int64_t(0), "-00000"); 
  test<int64_t>(int64_t(-2), "-000002"); 
  test<int64_t>(int64_t(0), "+0"); 
  test<int64_t>(int64_t(0), "+00"); 
  test<int64_t>(int64_t(10), "+010"); 
  test<int64_t>(int64_t(0), "+00000000"); 
  test<int64_t>(int64_t(2), "+000000002"); 
  test<int64_t>(int64_t(0), "+0000000000000000000000000000000000000000"); 
  test<int64_t>(int64_t(22), "+000000000000000000000000000000000000000022"); 
}

TEST(atoiTest, testPredefinedConstants) {
  test<int16_t>(INT16_MIN, std::to_string(INT16_MIN)); 
  test<int16_t>(INT16_MAX, std::to_string(INT16_MAX));

  test<int32_t>(INT32_MIN, std::to_string(INT32_MIN)); 
  test<int32_t>(INT32_MAX, std::to_string(INT32_MAX));
   
  test<int64_t>(INT64_MIN, std::to_string(INT64_MIN)); 
  test<int64_t>(INT64_MAX, std::to_string(INT64_MAX)); 

  test<uint8_t>(UINT8_MAX, std::to_string(UINT8_MAX)); 
  test<uint16_t>(UINT16_MAX, std::to_string(UINT16_MAX)); 
  test<uint32_t>(UINT32_MAX, std::to_string(UINT32_MAX)); 
  test<uint64_t>(UINT64_MAX, std::to_string(UINT64_MAX)); 
  
  test<std::size_t>(SIZE_MAX, std::to_string(SIZE_MAX));
}

TEST(atoiTest, testInvalidChars) {
  test<int64_t>(false, ""); 
  test<int64_t>(false, " "); 
  test<int64_t>(false, "  "); 
  test<int64_t>(false, "1a"); 
  test<int64_t>(false, "11234b"); 
  test<int64_t>(false, "1 "); 
  test<int64_t>(false, "1234 "); 
  test<int64_t>(false, "-"); 
  test<int64_t>(false, "+"); 
  test<int64_t>(false, "- "); 
  test<int64_t>(false, "+ "); 
  test<int64_t>(false, "-11234a"); 
  test<int64_t>(false, "-11234 "); 
  test<int64_t>(false, "o"); 
  test<int64_t>(false, "ooooo"); 
  test<int64_t>(false, "1A2B3C"); 
  test<int64_t>(false, "aaaaa14453"); 
  test<int64_t>(false, "02a"); 
}

TEST(atoiTest, testInt64OutOfBoundsLow) {
  // out of bounds
  test<int64_t>(false, "-1111111111111111111111111111111111111111111111111111111"); 
  test<int64_t>(false, "-111111111111111111111111111111111111111"); 
  test<int64_t>(false, "-9223372036854775810943"); 
  test<int64_t>(false, "-9223372036854775810"); 
  test<int64_t>(false, "-9223372036854775809"); 
}

TEST(atoiTest, testInt64InBounds) {
  // in bounds
  test<int64_t>(true,  "-9223372036854775808"); 
  test<int64_t>(true,  "-9223372036854775807");
  test<int64_t>(true,  "-9223372036854775801");
  test<int64_t>(true,  "-9223372036854775800");
  test<int64_t>(true,  "-9223372036854775799");
  test<int64_t>(true,  "-123456789012");
  test<int64_t>(true,  "-999999999");
  test<int64_t>(true,  "-98765543");
  test<int64_t>(true,  "-10000");
  test<int64_t>(true,  "-100");
  test<int64_t>(true,  "-99");
  test<int64_t>(true,  "-9");
  test<int64_t>(true,  "-2");
  test<int64_t>(true,  "-1");
  test<int64_t>(true,  "0");
  test<int64_t>(true,  "1");
  test<int64_t>(true,  "10");
  test<int64_t>(true,  "10000");
  test<int64_t>(true,  "1234567890");
  test<int64_t>(true,  "1844674407370955161");
  test<int64_t>(true,  "9223372036854775799"); 
  test<int64_t>(true,  "9223372036854775800"); 
  test<int64_t>(true,  "9223372036854775806"); 
  test<int64_t>(true,  "9223372036854775807"); 
}
  
TEST(atoiTest, testInt64OutOfBoundsHigh) {
  // out of bounds
  test<int64_t>(false, "9223372036854775808"); 
  test<int64_t>(false, "9223372036854775809"); 
  test<int64_t>(false, "18446744073709551610");
  test<int64_t>(false, "18446744073709551614");
  test<int64_t>(false, "18446744073709551615");
  test<int64_t>(false, "18446744073709551616");
  test<int64_t>(false, "118446744073709551612");
  test<int64_t>(false, "111111111111111111111111111111");
  test<int64_t>(false, "11111111111111111111111111111111111111111111111111111111111111111");
} 

TEST(atoiTest, testUint64OutOfBoundsNegative) {
  // out of bounds
  test<uint64_t>(false, "-1111111111111111111111111111111111111111111111111111111111111");
  test<uint64_t>(false, "-1111111111111111111111111111111111111");
  test<uint64_t>(false, "-9223372036854775809"); 
  test<uint64_t>(false, "-9223372036854775808");
  test<uint64_t>(false, "-9223372036854775807");
  test<uint64_t>(false, "-10000");
  test<uint64_t>(false, "-10000");
  test<uint64_t>(false, "-1");
  test<uint64_t>(false, "-0");
}

TEST(atoiTest, testUint64InBounds) {
  // in bounds
  test<uint64_t>(true,  "0");
  test<uint64_t>(true,  "1");
  test<uint64_t>(true,  "10");
  test<uint64_t>(true,  "10000");
  test<uint64_t>(true,  "1234567890");
  test<uint64_t>(true,  "9223372036854775807"); 
  test<uint64_t>(true,  "9223372036854775808"); 
  test<uint64_t>(true,  "1844674407370955161");
  test<uint64_t>(true,  "18446744073709551610");
  test<uint64_t>(true,  "18446744073709551614");
  test<uint64_t>(true,  "18446744073709551615");
}

TEST(atoiTest, testUint64OutOfBoundsHigh) {
  // out of bounds
  test<uint64_t>(false, "18446744073709551616");
  test<uint64_t>(false, "118446744073709551612");
  test<uint64_t>(false, "1111111111111111111111111111111111111");
  test<uint64_t>(false, "1111111111111111111111111111111111111111111111111111111111111");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
