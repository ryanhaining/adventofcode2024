#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace {
constexpr int SIZE = 71;
constexpr int BYTE_COUNT = 1024;

using Point = std::pair<long, long>;

enum class Direction { up, down, left, right };

constexpr std::array ALL_DIRECTIONS = {
    Direction::right,
    Direction::left,
    Direction::down,
    Direction::up,
};
Point adjust(Direction d, Point p) {
  switch (d) {
    case Direction::up:
      --p.first;
      break;
    case Direction::down:
      ++p.first;
      break;
    case Direction::left:
      --p.second;
      break;
    case Direction::right:
      ++p.second;
      break;
  }
  return p;
}

bool is_in_bounds(const Point& p) {
  return p.first >= 0 && p.second >= 0 && p.first < SIZE && p.second < SIZE;
}

struct Node {
  Point pos{};
  long distance{std::numeric_limits<long>::max()};
  bool is_corrupted{};
};

using Grid = std::vector<std::vector<Node>>;

class NodeHeap {
  std::vector<Node*> data_;

  static std::size_t parent_index(std::size_t index) { return (index - 1) / 2; }

  static bool is_lower_distance(const Node* lhs, const Node* rhs) {
    return lhs->distance < rhs->distance;
  }

  struct MinNodeByDistance {
    bool operator()(const Node* lhs, const Node* rhs) const {
      return is_lower_distance(rhs, lhs);
    }
  };

 public:
  void push(Node* n) {
    data_.push_back(n);
    std::push_heap(data_.begin(), data_.end(), MinNodeByDistance{});
  }

  Node* top() { return data_[0]; }

  void pop() {
    std::pop_heap(data_.begin(), data_.end(), MinNodeByDistance{});
    data_.pop_back();
  }

  bool empty() const { return data_.empty(); }

  void decrease_key(Node* n) {
    auto it = std::find(data_.begin(), data_.end(), n);
    if (it == data_.end()) {
      push(n);
      return;
    }

    std::size_t index = (it - data_.begin());
    while (index > 0 &&
           is_lower_distance(data_[index], data_[parent_index(index)])) {
      std::swap(data_[index], data_[parent_index(index)]);
      index = parent_index(index);
    }
  }
};

long find_shortest(NodeHeap& heap, Grid& grid, const Node* end) {
  while (true) {
    auto* n = heap.top();
    heap.pop();
    assert(n->distance != std::numeric_limits<long>::max());
    if (n == end) {
      return n->distance;
    }
    auto next_dist = n->distance + 1;
    for (auto d : ALL_DIRECTIONS) {
      auto next_pos = adjust(d, n->pos);
      if (is_in_bounds(next_pos)) {
        auto* next = &grid[next_pos.first][next_pos.second];
        if (!next->is_corrupted && next_dist < next->distance) {
          next->distance = next_dist;
          heap.decrease_key(next);
        }
      }
    }
  }
}

}  // namespace

int main() {
  std::size_t col{};
  std::size_t row{};
  Grid mem_space(SIZE, std::vector<Node>(SIZE));
  for (std::size_t row = 0; row < mem_space.size(); ++row) {
    for (std::size_t col = 0; col < mem_space[row].size(); ++col) {
      mem_space[row][col].pos = Point{row, col};
    }
  }
  int byte_count{};
  char ignore;
  while (byte_count != BYTE_COUNT && (std::cin >> col >> ignore >> row)) {
    mem_space[row][col].is_corrupted = true;
    ++byte_count;
  }
  assert(byte_count == BYTE_COUNT);

  mem_space.front().front().distance = 0;
  NodeHeap heap;
  for (auto& row : mem_space) {
    for (auto& n : row) {
      if (!n.is_corrupted) {
        heap.push(&n);
      }
    }
  }

  std::cout << find_shortest(heap, mem_space, &mem_space.back().back()) << '\n';
}
