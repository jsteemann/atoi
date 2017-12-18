String to integer conversion
----------------------------

This is a header-only library for converting a string containing a
base-10 number into an integer value of a configurable type T.  

The input string is considered valid only if it consists of the digits
'0' to '9'. An optional '+' or '-' sign is allowed at the very beginning 
too. If any other character is found, the input is considered invalid.
The input string does not need to be null-terminated.

If the parsed number value would be less or greater than what the 
number type T can store without truncation, the input is considered
invalid and parsing is stopped.

The library makes a few assumptions about the input strings in order to 
speed up the parsing:

* it will treat all input as base-10 numbers
* it is optimized for valid input, i.e. strings that contain only the 
  digits '0' to '9' (with an optional '+' or '-' sign in front)
* it will not modify `errno` in any way
* it will not throw exceptions in case the input is invalid or the
  resulting integer value would out of bounds

Example usage
-------------

```cpp
#include "jsteemann/atoi.h"
#include <iostream>

// the string we will parse
std::string value("12345678901234");

bool valid;
auto result = jsteemann::atoi<uint64_t>(value.data(), value.size(), valid);

if (valid) {
  // parsing succeeded!
  std::cout << "successfully parsed '" << value << "' into number " << result << std::endl;
} else {
  // parsing failed!
  std::cout << "failed to parse '" << value << "' into a number!" << std::endl;
}
```

The library contains the following functions:
```cpp
// function to convert the string value between p 
// (inclusive) and e (exclusive) into a number value of type T
//
// the input string will always be interpreted as a base-10 number.
// expects the input string to contain only the digits '0' to '9'. an
// optional '+' or '-' sign is allowed too. 
// if any other character is found, the output parameter "valid" will 
// be set to false. if the parsed value is less or greater than what 
// type T can store without truncation, "valid" will also be set to 
// false.
// this function will not modify errno.
template<typename T>
static inline T atoi(char const* p, std::size_t length, bool& valid) noexcept;

// low-level worker function to convert the string value between p 
// (inclusive) and e (exclusive) into a positive number value of type T
//
// the input string will always be interpreted as a base-10 number.
// expects the input string to contain only the digits '0' to '9'. 
// if any other character is found, the output parameter "valid" will 
// be set to false. if the parsed value is greater than what type T can
// store without truncation, "valid" will also be set to false.
// this function will always read *p, so it expects p to be != e.
// this function will not modify errno.
template<typename T>
static inline T atoi_positive(char const* p, char const* e, bool& valid) noexcept;
```

Benchmark
---------

This library's string-to-integer conversion is not as flexible as
the functionality provided by the standard library. It sacrifices a bit
of the generality for performance. It is also optimized for valid input
strings.

To compare the performance of this library and the standard library's
`std::stoull` function and plus stdlib.h's `strtoull` function, there is
a benchmark executable included.

It can be built and run as follows:
```bash
mkdir -p build
# be sure to build in Release mode here for compiler optimizations
(cd build && cmake -DCMAKE_BUILD_TYPE=Release ..)
build/benchmark/bench
```

Benchmark results from local laptop (Linux x86-64):
```
500000000 iterations of std::stoull, string '7' took 4781 ms
500000000 iterations of strtoull, string '7' took 4476 ms
500000000 iterations of jsteemann::atoi, string '7' took 1007 ms
500000000 iterations of jsteemann::atoi_positive, string '7' took 863 ms

500000000 iterations of std::stoull, string '874' took 6493 ms
500000000 iterations of strtoull, string '874' took 6231 ms
500000000 iterations of jsteemann::atoi, string '874' took 1973 ms
500000000 iterations of jsteemann::atoi_positive, string '874' took 1730 ms

500000000 iterations of std::stoull, string '123456' took 9156 ms
500000000 iterations of strtoull, string '123456' took 8834 ms
500000000 iterations of jsteemann::atoi, string '123456' took 3281 ms
500000000 iterations of jsteemann::atoi_positive, string '123456' took 4004 ms

500000000 iterations of std::stoull, string '12345654666646' took 16336 ms
500000000 iterations of strtoull, string '12345654666646' took 15909 ms
500000000 iterations of jsteemann::atoi, string '12345654666646' took 6632 ms
500000000 iterations of jsteemann::atoi_positive, string '12345654666646' took 8636 ms

500000000 iterations of std::stoull, string '16323949897939569634' took 21711 ms
500000000 iterations of strtoull, string '16323949897939569634' took 21513 ms
500000000 iterations of jsteemann::atoi, string '16323949897939569634' took 13792 ms
500000000 iterations of jsteemann::atoi_positive, string '16323949897939569634' took 14681 ms
```

Tests
-----

To run the library's tests locally, execute the following commands:

```bash
mkdir -p build
(cd build && cmake ..)
build/tests/tests
```
