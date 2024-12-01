#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <ranges>

int main() {
  int n1{};
  int n2{};
  std::vector<int> left;
  std::vector<int> right;
  while (std::cin >> n1 >> n2) {
    left.push_back(n1);
    right.push_back(n2);
  }
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  auto result = std::ranges::fold_left(
      std::ranges::views::zip_transform(
          [](int n1, int n2) { return std::abs(n1 - n2); },
          std::ranges::views::all(left), std::ranges::views::all(right)),
      0, std::plus{});

  std::cout << result << '\n';
}
