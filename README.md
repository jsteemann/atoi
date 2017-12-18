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

