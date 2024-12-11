#include <cmath>
#include <iostream>
#include <iterator>
#include <map>

namespace {
constexpr int NUM_BLINKS = 75;
constexpr long MUL = 2024;

long get_num_digits(long n) {
  auto log = std::log10(n);
  return static_cast<long>(std::ceil(log + 0.00001));
}

class LenCalc {
 public:
  long operator()(long n, int blinks) {
    if (blinks == 0) {
      return 1;
    }
    auto it = cache_.find({n, blinks});
    if (it != cache_.end()) {
      return it->second;
    }
    if (n == 0) {
      auto result = (*this)(1, blinks - 1);
      cache_[{n, blinks}] = result;
      return result;
    }
    auto num_digits = get_num_digits(n);
    if (num_digits % 2 == 0) {
      auto denom = static_cast<long>(std::pow(10, num_digits / 2));
      auto result =
          (*this)(n / denom, blinks - 1) + (*this)(n % denom, blinks - 1);
      cache_[{n, blinks}] = result;
      return result;
    }
    auto result = (*this)(n * MUL, blinks - 1);
    cache_[{n, blinks}] = result;
    return result;
  }

 private:
  std::map<std::pair<long, int>, long> cache_;
};

}  // namespace

int main() {
  long total{};
  LenCalc get_len{};
  for (auto it = std::istream_iterator<long>{std::cin};
       it != std::istream_iterator<long>{}; ++it) {
    total += get_len(*it, NUM_BLINKS);
  }
  std::cout << total << '\n';
}
