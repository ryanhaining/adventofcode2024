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

long possible_ways(std::unordered_map<std::string, long>& cache,
                   const std::vector<std::string>& towels,
                   const std::string& design) {
  if (design.empty()) {
    return 1;
  }
  if (auto it = cache.find(design); it != cache.end()) {
    return it->second;
  }
  long count = 0;
  for (const auto& t : towels) {
    if (design.starts_with(t)) {
      count += possible_ways(cache, towels, design.substr(t.size()));
    }
  }
  return cache[design] = count;
}

}  // namespace

int main() {
  auto towels = read_towels();
  std::unordered_map<std::string, long> cache;
  std::cin.ignore(1024, '\n');  // ignore blank link
  std::string design{};
  long total{};
  while (std::getline(std::cin, design)) {
    total += possible_ways(cache, towels, design);
  }
  std::cout << total << '\n';
}
