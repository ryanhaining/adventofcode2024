#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

int main() {
  int n1{};
  int n2{};
  std::vector<int> nums;
  std::unordered_map<int, int> counts;
  while (std::cin >> n1 >> n2) {
    nums.push_back(n1);
    ++counts[n2];
  }

  auto result = std::ranges::fold_left(
      std::ranges::views::transform(std::ranges::views::all(nums),
                                    [&counts](int n) { return n * counts[n]; }),
      0, std::plus{});

  std::cout << result << '\n';
}
