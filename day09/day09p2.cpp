#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <queue>

namespace {
constexpr unsigned int MAX_SIZE = 9;

struct Block {
  std::size_t index;
  unsigned int size;
  std::size_t f_id;
};

using FreeBlocks =
    std::map<int, std::priority_queue<std::size_t, std::vector<std::size_t>,
                                      std::greater<std::size_t>>>;

// Returns size of the first block that can fit `size` bytes or 0 if none exist.
unsigned int find_first_free(unsigned int size, FreeBlocks& free_blocks) {
  std::vector<std::pair<unsigned int, std::size_t>> options;
  unsigned int min_size = std::numeric_limits<unsigned int>::max();
  std::size_t min_index = std::numeric_limits<std::size_t>::max();
  // std::map has keys in sorted order
  for (auto it = free_blocks.find(size); it != free_blocks.end(); ++it) {
    if (!it->second.empty()) {
      auto index = it->second.top();
      if (index < min_index) {
        min_size = it->first;
        min_index = index;
      }
    }
  }
  if (min_index != std::numeric_limits<std::size_t>::max()) {
    return min_size;
  }
  return 0;
}

}  // namespace

int main() {
  FreeBlocks free_blocks;
  for (std::size_t i = 0; i <= MAX_SIZE; ++i) {
    free_blocks[i];
  }
  std::vector<Block> fs{};

  char digit{};
  std::size_t counter{};
  bool free_flag = false;
  std::size_t index{};
  while (std::cin >> digit) {
    unsigned int n = digit - '0';
    if (free_flag) {
      if (n) {
        free_blocks[n].push(index);
      }
    } else {
      fs.emplace_back(index, n, counter);
      ++counter;
    }
    index += n;
    free_flag = !free_flag;
  }

  for (auto it = fs.rbegin(); it != fs.rend(); ++it) {
    auto& block = *it;
    auto free_block_size = find_first_free(block.size, free_blocks);
    if (free_block_size == 0) {
      continue;
    }
    auto free_block_index = free_blocks[free_block_size].top();
    if (free_block_index > block.index) {
      continue;
    }
    block.index = free_block_index;
    auto new_free_size = free_block_size - block.size;
    auto new_free_index = free_block_index + block.size;
    free_blocks[free_block_size].pop();
    if (new_free_size) {
      free_blocks[new_free_size].push(new_free_index);
    }
  }

  std::size_t total{};
  for (const auto& b : fs) {
    auto low = b.index;
    auto high = b.index + b.size - 1;
    // sum of range
    total += b.f_id * (((low + high) * (high - low + 1)) / 2);
  }
  std::cout << total << '\n';
}
