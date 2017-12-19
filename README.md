String to integer conversion library
------------------------------------

This is a header-only library for converting a string containing a
base-10 number into an integer value of a configurable type T.  

The library offers functions to parse a string into an integer with
validation of the input string or without validation. It is up to the
embedder to pick the appropriate function.

If the validating functions are used, the input string is considered 
valid only if it consists of the digits '0' to '9'. An optional '+' 
or '-' sign is allowed at the very beginning of the input string too. 
If any other character is found, the input is considered invalid.
The input string does not need to be null-terminated.

If the parsed number value would be less or greater than what the 
number type T can store without truncation, the input is considered
invalid and parsing is stopped.

The non-validating functions do not validate the input string for
validity nor do they check for overflow or underflow of the result
value.

The library makes a few assumptions about the input in order to provide 
good performance:

* all input is treated as base-10 numbers - no support for hexadecimal
  or octal numbers nor for floating point values
* the library functions are optimized for valid input, i.e. strings that 
  contain only the digits '0' to '9' (with an optional '+' or '-' sign in 
  front). This is also true for the validating functions
* the library functions will not modify `errno` in any way, nor will they
  throw any exceptions
* the library functions will not allocate any memory on the heap

In contrast to other common string-to-integer functions, the functions
of this library do not require null-terminated input strings. An input
string is delimited simply by a start pointer (`char const* p`) and end 
pointer (`char const* e`) into its data. All library functions guarantee
to only read memory between `p` (inclusive) and `e` (exclusive).

Use cases
---------

This library's string-to-integer conversion functionality is not as flexible 
as the one provided by other string-to-integer functions, e.g. `std::stoull`
from the standard library or `strtoull` from `stdlib.h`. 

This library sacrifices some of the generality for performance. It is also 
optimized for valid input strings, and provides special functions that do not 
validate the input at all. Embedders can use these functions when they know
the input strings are valid and will not overflow the target datatype.

Example usage
-------------

```cpp
#include "jsteemann/atoi.h"
#include <iostream>

// the string to be parsed
std::string value("12345678901234");

bool valid;
auto result = jsteemann::atoi<uint64_t>(value.data(), value.data() + value.size(), valid);

if (valid) {
  // parsing succeeded!
  std::cout << "successfully parsed '" << value << "' into number " << result << std::endl;
} else {
  // parsing failed!
  std::cout << "failed to parse '" << value << "' into a number!" << std::endl;
}
```

The library contains the following validating functions:
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
// false. In this case the returned result should not be used.
// this function will not modify errno.
template<typename T>
static inline T atoi(char const* p, char const* e, bool& valid) noexcept;

// low-level worker function to convert the string value between p 
// (inclusive) and e (exclusive) into a positive number value of type T
//
// the input string will always be interpreted as a base-10 number.
// expects the input string to contain only the digits '0' to '9'. 
// if any other character is found, the output parameter "valid" will 
// be set to false. if the parsed value is greater than what type T can
// store without truncation, "valid" will also be set to false. In this
// case the returned result should not be used.
// this function will not modify errno.
template<typename T>
static inline T atoi_positive(char const* p, char const* e, bool& valid) noexcept;
```

The library contains the following non-validating functions:
```cpp
// function to convert the string value between p 
// (inclusive) and e (exclusive) into a number value of type T, without
// validation of the input string - use this only for trusted input!
//
// the input string will always be interpreted as a base-10 number.
// expects the input string to contain only the digits '0' to '9'. an
// optional '+' or '-' sign is allowed too. 
// there is no validation of the input string, and overflow or underflow
// of the result value will not be detected.
// this function will not modify errno.
template<typename T>
inline T atoi_unchecked(char const* p, char const* e) noexcept;

// low-level worker function to convert the string value between p 
// (inclusive) and e (exclusive) into a positive number value of type T,
// without validation of the input string - use this only for trusted input!
//
// the input string will always be interpreted as a base-10 number.
// expects the input string to contain only the digits '0' to '9'. 
// there is no validation of the input string, and overflow or underflow
// of the result value will not be detected.
// this function will not modify errno.
template<typename T>
inline T atoi_positive_unchecked(char const* p, char const* e) noexcept;
```

Benchmark
---------

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
500000000 iterations of std::stoull, string '7' took 4773 ms
500000000 iterations of strtoull, string '7' took 4478 ms
500000000 iterations of jsteemann::atoi, string '7' took 976 ms
500000000 iterations of jsteemann::atoi_positive, string '7' took 871 ms
500000000 iterations of jsteemann::atoi_positive_unchecked, string '7' took 888 ms

500000000 iterations of std::stoull, string '874' took 6502 ms
500000000 iterations of strtoull, string '874' took 6230 ms
500000000 iterations of jsteemann::atoi, string '874' took 2215 ms
500000000 iterations of jsteemann::atoi_positive, string '874' took 1747 ms
500000000 iterations of jsteemann::atoi_positive_unchecked, string '874' took 1312 ms

500000000 iterations of std::stoull, string '123456' took 9151 ms
500000000 iterations of strtoull, string '123456' took 8892 ms
500000000 iterations of jsteemann::atoi, string '123456' took 3902 ms
500000000 iterations of jsteemann::atoi_positive, string '123456' took 3081 ms
500000000 iterations of jsteemann::atoi_positive_unchecked, string '123456' took 2312 ms

500000000 iterations of std::stoull, string '12345654666646' took 16269 ms
500000000 iterations of strtoull, string '12345654666646' took 16006 ms
500000000 iterations of jsteemann::atoi, string '12345654666646' took 8928 ms
500000000 iterations of jsteemann::atoi_positive, string '12345654666646' took 6542 ms
500000000 iterations of jsteemann::atoi_positive_unchecked, string '12345654666646' took 4698 ms

500000000 iterations of std::stoull, string '16323949897939569634' took 21766 ms
500000000 iterations of strtoull, string '16323949897939569634' took 21524 ms
500000000 iterations of jsteemann::atoi, string '16323949897939569634' took 16579 ms
500000000 iterations of jsteemann::atoi_positive, string '16323949897939569634' took 13696 ms
500000000 iterations of jsteemann::atoi_positive_unchecked, string '16323949897939569634' took 6742 ms
```

Tests
-----

To run the library's tests locally, execute the following commands:

```bash
mkdir -p build
(cd build && cmake ..)
build/tests/tests
```
