#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
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

std::istream& operator>>(std::istream& in, Point& p) {
  in >> p.second;
  in.ignore(1);
  in >> p.first;
  return in;
}

int main() {
  const Grid clean(SIZE, std::vector<TileState>(SIZE, TileState::free));
  Point p{};
  std::vector<Point> corrupt_points;
  while (std::cin >> p) {
    corrupt_points.push_back(p);
  }

  std::size_t known_good{0};
  std::size_t known_bad{corrupt_points.size()};
  while (known_good != known_bad - 1) {
    auto grid = clean;
    auto end = (known_good + known_bad) / 2;
    for (std::size_t i = 0; i < end; ++i) {
      auto p = corrupt_points[i];
      grid[p.first][p.second] = TileState::corrupted;
    }
    if (has_path(std::move(grid))) {
      known_good = end;
    } else {
      known_bad = end;
    }
  }
  std::cout << corrupt_points[known_bad - 1].second << ','
            << corrupt_points[known_bad - 1].first << '\n';
}
