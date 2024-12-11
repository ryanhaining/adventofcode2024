#include <cmath>
#include <forward_list>
#include <iostream>
#include <iterator>

namespace {
constexpr int NUM_BLINKS = 25;
constexpr long MUL = 2024;

long get_num_digits(long n) {
  auto log = std::log10(n);
  return static_cast<long>(std::ceil(log + 0.00001));
}

}  // namespace

int main() {
  std::forward_list<long> nums(std::istream_iterator<long>{std::cin},
                               std::istream_iterator<long>{});
  for (int i = 0; i < NUM_BLINKS; ++i) {
    for (auto it = nums.begin(); it != nums.end(); ++it) {
      if (*it == 0) {
        *it = 1;
      } else if (auto num_digits = get_num_digits(*it); num_digits % 2 == 0) {
        auto denom = static_cast<long>(std::pow(10, num_digits / 2));
        long n1 = *it / denom;
        long n2 = *it % denom;
        *it = n1;
        nums.insert_after(it, n2);
        ++it;
      } else {
        (*it) *= MUL;
      }
    }
  }
  std::cout << std::distance(nums.begin(), nums.end()) << '\n';
}
