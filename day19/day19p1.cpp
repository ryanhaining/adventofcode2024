#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

std::vector<std::string> read_towels() {
  std::string towels_line{};
  assert(std::getline(std::cin, towels_line));
  towels_line += ',';
  std::istringstream towels_stream{towels_line};

  std::vector<std::string> towels(
      std::istream_iterator<std::string>{towels_stream},
      std::istream_iterator<std::string>{});

  for (auto& t : towels) {
    t.pop_back();
  }
  return towels;
}

bool is_possible(std::unordered_map<std::string, bool>& cache,
                 const std::vector<std::string>& towels,
                 const std::string& design) {
  if (design.empty()) {
    return true;
  }
  if (auto it = cache.find(design); it != cache.end()) {
    return it->second;
  }
  for (const auto& t : towels) {
    if (design.starts_with(t) &&
        is_possible(cache, towels, design.substr(t.size()))) {
      return cache[design] = true;
    }
  }
  return cache[std::string{design}] = false;
}

}  // namespace

int main() {
  auto towels = read_towels();
  std::unordered_map<std::string, bool> cache;
  std::cin.ignore(1024, '\n');  // ignore blank link
  std::string design{};
  int total{};
  while (std::getline(std::cin, design)) {
    total += is_possible(cache, towels, design);
  }
  std::cout << total << '\n';
}
