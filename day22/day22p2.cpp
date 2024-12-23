#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <map>
#include <set>

namespace {

constexpr unsigned long PRUNE_VAL = 16777216;
constexpr int STEPS = 2000;
constexpr int HISTORY_LEN = 4;

unsigned long step(unsigned long n) {
  n = (n ^ (n * 64)) % PRUNE_VAL;
  n = (n ^ (n / 32)) % PRUNE_VAL;
  n = (n ^ (n * 2048)) % PRUNE_VAL;
  return n;
}

using History = std::array<int, HISTORY_LEN>;

void map_prices(std::map<History, int>& totals, unsigned long secret) {
  std::set<History> seen;
  std::deque<int> prev_changes = {};
  for (int i = 0; i < STEPS; ++i) {
    auto new_secret = step(secret);
    if (prev_changes.size() == HISTORY_LEN) {
      prev_changes.pop_front();
    }
    prev_changes.push_back(static_cast<int>(new_secret) % 10 -
                           static_cast<int>(secret) % 10);
    secret = new_secret;
    if (prev_changes.size() == HISTORY_LEN) {
      auto key = History{};
      std::copy(prev_changes.begin(), prev_changes.end(), key.begin());
      if (!seen.contains(key)) {
        totals[key] += secret % 10;
        seen.insert(key);
      }
    }
  }
}

}  // namespace

int main() {
  unsigned long secret{};
  std::map<History, int> totals;
  while (std::cin >> secret) {
    map_prices(totals, secret);
  }
  auto it = std::max_element(
      totals.begin(), totals.end(),
      [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
  std::cout << it->second << '\n';
}
