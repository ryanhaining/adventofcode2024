#include <cmath>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace {

long concat_longs(long first, long second) {
  int places = static_cast<int>(std::log10(second)) + 1;
  int magnitude = static_cast<int>(std::pow(10, places));
  return first * magnitude + second;
}

bool has_solution(long ans, long so_far,
                  std::vector<long>::const_iterator current,
                  std::vector<long>::const_iterator end) {
  if (so_far > ans) {
    return false;
  }
  if (current == end) {
    return ans == so_far;
  }
  return has_solution(ans, so_far + *current, current + 1, end) ||
         has_solution(ans, so_far * *current, current + 1, end) ||
         has_solution(ans, concat_longs(so_far, *current), current + 1, end);
}
long line_val(const std::string& line) {
  std::istringstream line_stream{line};
  long ans{};
  line_stream >> ans;
  line_stream.ignore(1);
  std::vector<long> nums(std::istream_iterator<long>{line_stream},
                         std::istream_iterator<long>{});

  return has_solution(ans, *nums.begin(), nums.begin() + 1, nums.end()) ? ans
                                                                        : 0;
}

}  // namespace

int main() {
  long total{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    total += line_val(line);
  }

  std::cout << total << '\n';
}
