#include <iostream>
#include <chrono>
#include <functional>
#include <string>

// for strtoull
#include <stdlib.h>

#include "jsteemann/atoi.h"
  
static unsigned long long measure(std::string const& what, std::function<unsigned long long()> const& cb) {
  typedef std::chrono::high_resolution_clock clock;
  std::chrono::time_point<clock> start = clock::now();

  unsigned long long result = cb();

  std::cout << what << " took "
            << std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count() << " ms" 
            << std::endl;

  return result;
}

int main() {
  std::size_t const n = 500 * 1000 * 1000;
  std::string value;

  // callback functions for the benchmark
   
  // using std::stoull
  auto func1 = [n, &value]() {
    volatile unsigned long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
      std::size_t nc;
      auto result = std::stoull(value, &nc);
      // in here so compiler does not optimize it away
      total += result;
    }
    return total;
  };
  
  // using strtoull from stdlib.h
  auto func2 = [n, &value]() {
    volatile unsigned long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
      char* endptr;
      auto result = ::strtoull(value.c_str(), &endptr, 10);
      // in here so compiler does not optimize it away
      total += result;
    }
    return total;
  };
  
  // using jsteemann::atoi
  auto func3 = [n, &value]() {
    volatile unsigned long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
      bool valid;
      auto result = jsteemann::atoi<unsigned long long>(value.data(), value.data() + value.size(), valid);
      // in here so compiler does not optimize it away
      total += result;
    }
    return total;
  };
  
  
  // using jsteemann::atoi_positive
  auto func4 = [n, &value]() {
    volatile unsigned long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
      bool valid;
      auto result = jsteemann::atoi_positive<unsigned long long>(value.data(), value.data() + value.size(), valid);
      // in here so compiler does not optimize it away
      total += result;
    }
    return total;
  };
  
  
  // using jsteemann::atoi_positive_unchecked
  auto func5 = [n, &value]() {
    volatile unsigned long long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
      auto result = jsteemann::atoi_positive_unchecked<unsigned long long>(value.data(), value.data() + value.size());
      // in here so compiler does not optimize it away
      total += result;
    }
    return total;
  };
  
  
  // use a very short value for starting 
  value = "7";
  (void) measure(std::to_string(n) + " iterations of std::stoull, string '" + value + "'", func1);
  (void) measure(std::to_string(n) + " iterations of strtoull, string '" + value + "'", func2);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi, string '" + value + "'", func3);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive, string '" + value + "'", func4);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive_unchecked, string '" + value + "'", func5);
  std::cout << std::endl;
  
  
  // three bytes now
  value = "874";
  (void) measure(std::to_string(n) + " iterations of std::stoull, string '" + value + "'", func1);
  (void) measure(std::to_string(n) + " iterations of strtoull, string '" + value + "'", func2);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi, string '" + value + "'", func3);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive, string '" + value + "'", func4);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive_unchecked, string '" + value + "'", func5);
  std::cout << std::endl;
    

  // try a slightly longer value now
  value = "123456";
  (void) measure(std::to_string(n) + " iterations of std::stoull, string '" + value + "'", func1);
  (void) measure(std::to_string(n) + " iterations of strtoull, string '" + value + "'", func2);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi, string '" + value + "'", func3);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive, string '" + value + "'", func4);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive_unchecked, string '" + value + "'", func5);
  std::cout << std::endl;
  
  
  // it only gets longer
  value = "12345654666646";
  (void) measure(std::to_string(n) + " iterations of std::stoull, string '" + value + "'", func1);
  (void) measure(std::to_string(n) + " iterations of strtoull, string '" + value + "'", func2);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi, string '" + value + "'", func3);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive, string '" + value + "'", func4);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive_unchecked, string '" + value + "'", func5);
  std::cout << std::endl;
  
  
  // try a rather long value now
  value = "16323949897939569634";
  (void) measure(std::to_string(n) + " iterations of std::stoull, string '" + value + "'", func1);
  (void) measure(std::to_string(n) + " iterations of strtoull, string '" + value + "'", func2);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi, string '" + value + "'", func3);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive, string '" + value + "'", func4);
  (void) measure(std::to_string(n) + " iterations of jsteemann::atoi_positive_unchecked, string '" + value + "'", func5);
  std::cout << std::endl;
}
