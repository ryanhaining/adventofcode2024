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

enum class TileState {
  free,
  corrupted,
  visited,
};

using Grid = std::vector<std::vector<TileState>>;

void flood_fill(Grid& grid, Point current) {
  if (!is_in_bounds(current) ||
      grid[current.first][current.second] != TileState::free) {
    return;
  }
  grid[current.first][current.second] = TileState::visited;
  for (auto d : ALL_DIRECTIONS) {
    flood_fill(grid, adjust(d, current));
  }
}

bool has_path(Grid grid) {
  flood_fill(grid, Point{0, 0});
  return grid.back().back() == TileState::visited;
}

}  // namespace

int main() {
  std::size_t col{};
  std::size_t row{};
  Grid grid(SIZE, std::vector<TileState>(SIZE, TileState::free));
  char ignore;
  while (std::cin >> col >> ignore >> row) {
    grid[row][col] = TileState::corrupted;
    if (!has_path(grid)) {
      std::cout << col << ',' << row << '\n';
      break;
    }
  }
}
