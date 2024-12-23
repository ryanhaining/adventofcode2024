#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace {

enum class Direction { up, down, left, right, press };

struct Point {
  int row{};
  int col{};
};

struct Delta {
  int row_diff{};
  int col_diff{};

  bool up_before_left{};
  bool right_before_down{};
  bool down_before_left{};
  bool right_before_up{};

  auto operator<=>(const Delta& other) const = default;

  int distance() const { return std::abs(row_diff) + std::abs(col_diff); }

  // left, down, up, right
  //
  // idc, I'm just glad this works and I didn't have to hardcode the paths.
  std::vector<Direction> moves() const {
    std::vector<Direction> result;
    if (up_before_left) {
      append_dir(result, Direction::up, up());
    }
    if (right_before_down) {
      append_dir(result, Direction::right, right());
    }
    if (down_before_left) {
      append_dir(result, Direction::down, down());
    }
    append_dir(result, Direction::left, left());
    if (!down_before_left) {
      append_dir(result, Direction::down, down());
    }
    if (right_before_up) {
      append_dir(result, Direction::right, right());
    }
    if (!up_before_left) {
      append_dir(result, Direction::up, up());
    }
    if (!right_before_down && !right_before_up) {
      append_dir(result, Direction::right, right());
    }
    result.push_back(Direction::press);
    return result;
  }

  static void append_dir(std::vector<Direction>& dirs, Direction d, int count) {
    for (int i = 0; i < count; ++i) {
      dirs.push_back(d);
    }
  }

  int up() const { return row_diff < 0 ? -row_diff : 0; }
  int down() const { return row_diff > 0 ? row_diff : 0; }
  int left() const { return col_diff < 0 ? -col_diff : 0; }
  int right() const { return col_diff > 0 ? col_diff : 0; }
};

Delta operator-(Point p1, Point p2) {
  return {p1.row - p2.row, p1.col - p2.col};
}

Point& operator+=(Point& p, const Delta& d) {
  p.row += d.row_diff;
  p.col += d.col_diff;
  return p;
}

constexpr Point PAD_A_LOC{3, 2};

const std::unordered_map<char, Point> PAD_KEYS = {
    {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}},    {'4', {1, 0}},
    {'5', {1, 1}}, {'6', {1, 2}}, {'1', {2, 0}},    {'2', {2, 1}},
    {'3', {2, 2}}, {'0', {3, 1}}, {'A', PAD_A_LOC},
};

constexpr Point DIR_A_LOC{0, 2};

const std::unordered_map<Direction, Point> DIR_KEYS = {
    {Direction::up, {0, 1}},       {Direction::left, {1, 0}},
    {Direction::down, {1, 1}},     {Direction::right, {1, 2}},
    {Direction::press, DIR_A_LOC},
};

// global variable, sucks, whatever, this whole thing is a mess.
std::map<std::pair<Delta, int>, long> length_cache;

long expand_move(const Delta& move, int depth) {
  if (depth == 0) {
    return move.moves().size();
  }
  if (auto it = length_cache.find({move, depth}); it != length_cache.end()) {
    return it->second;
  }
  auto pos = DIR_A_LOC;
  long length{};
  for (const auto dir : move.moves()) {
    auto diff = DIR_KEYS.at(dir) - pos;
    if (pos.row == 0 && DIR_KEYS.at(dir).col == 0) {
      diff.down_before_left = true;
    }
    if (pos.col == 0 && DIR_KEYS.at(dir).row == 0) {
      diff.right_before_up = true;
    }
    length += expand_move(diff, depth - 1);
    pos += diff;
  }

  return length_cache[{move, depth}] = length;
}

long process(std::string code) {
  auto pos = PAD_A_LOC;
  std::vector<Delta> l1_moves;
  for (auto c : code) {
    auto diff = PAD_KEYS.at(c) - pos;
    if (pos.row == 3 && PAD_KEYS.at(c).col == 0) {
      diff.up_before_left = true;
    }
    if (pos.col == 0 && PAD_KEYS.at(c).row == 3) {
      diff.right_before_down = true;
    }
    l1_moves.push_back(diff);
    pos += diff;
  }

  long total{};
  for (const auto& m : l1_moves) {
    total += expand_move(m, 25);
  }
  code.pop_back();
  return total * std::stol(code);
}
}  // namespace

int main() {
  std::string code{};
  long total{};
  while (std::getline(std::cin, code)) {
    total += process(code);
  }
  std::cout << total << '\n';
}
