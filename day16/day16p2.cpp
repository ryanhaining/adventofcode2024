#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

constexpr long TURN_PENALTY = 1000;

using Point = std::pair<std::size_t, std::size_t>;
using Grid = std::vector<std::string>;

enum class Direction { up, down, left, right };

constexpr std::array ALL_DIRECTIONS = {
    Direction::right,
    Direction::left,
    Direction::down,
    Direction::up,
};

Direction opposite(Direction d) {
  switch (d) {
    case Direction::up:
      return Direction::down;
    case Direction::down:
      return Direction::up;
    case Direction::left:
      return Direction::right;
    case Direction::right:
      return Direction::left;
  }
  assert(false);
}

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

enum class Orientation { vertical, horizontal };

Orientation other(Orientation orientation) {
  switch (orientation) {
    case Orientation::horizontal:
      return Orientation::vertical;
    case Orientation::vertical:
      return Orientation::horizontal;
  }
  assert(false);
}

Orientation orientation_for(Direction d) {
  switch (d) {
    case Direction::up:
    case Direction::down:
      return Orientation::vertical;
    case Direction::left:
    case Direction::right:
      return Orientation::horizontal;
  }
  assert(false);
}

struct Node;

struct Edge {
  Node* src{};
  Node* dest{};
  Orientation orientation;
  long length{};
};

struct DistanceHistory {
  long distance{std::numeric_limits<long>::max()};
  std::vector<std::vector<const Edge*>> edge_paths = {{}};
};

struct Node {
  std::unordered_map<Orientation, DistanceHistory> distances = {
      {Orientation::vertical, {}},
      {Orientation::horizontal, {}},
  };
  std::vector<Edge> edges;
  long min_distance() const {
    return std::min(distances.at(Orientation::vertical).distance,
                    distances.at(Orientation::horizontal).distance);
  }
};

struct PointHash {
  std::size_t operator()(const Point& p) const {
    return std::hash<std::size_t>{}(p.first) ^
           (std::hash<std::size_t>{}(p.second) << 1);
  }
};

using PointToNode = std::unordered_map<Point, Node, PointHash>;

bool is_lower_distance(const Node* lhs, const Node* rhs) {
  return lhs->min_distance() < rhs->min_distance();
}

struct MinNodeByDistance {
  bool operator()(const Node* lhs, const Node* rhs) const {
    return is_lower_distance(rhs, lhs);
  }
};

std::size_t parent_index(std::size_t index) { return (index - 1) / 2; }

class NodeHeap {
  std::vector<Node*> data_;

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

constexpr std::string FREE_PATH_CHARS = ".SE";
constexpr std::string PATH_CHARS = ".XSE";

bool is_in_bounds(const Grid& grid, const Point& p) {
  return p.first > 0 && p.first < grid.size() && p.second > 0 &&
         p.second < grid[0].size();
}

bool is_path(const Grid& grid, const Point& p) {
  return is_in_bounds(grid, p) && PATH_CHARS.contains(grid[p.first][p.second]);
}

bool is_free_path(const Grid& grid, const Point& p) {
  return is_in_bounds(grid, p) &&
         FREE_PATH_CHARS.contains(grid[p.first][p.second]);
}

bool is_vertex(const Grid& grid, Point p, Direction dir) {
  return is_path(grid, p) &&
         (!is_path(grid, adjust(dir, p)) ||
          std::count_if(ALL_DIRECTIONS.begin(), ALL_DIRECTIONS.end(),
                        [&p, &grid](Direction d) {
                          return is_path(grid, adjust(d, p));
                        }) > 2);
}

void collect_vertexes_impl(Grid& grid, PointToNode& all_nodes, Node* prev,
                           Point current, Direction dir, long distance) {
  if (!is_path(grid, current)) {
    return;
  }
  if (all_nodes.contains(current) || is_vertex(grid, current, dir)) {
    auto& n = all_nodes[current];
    if (&n == prev) {
      return;
    }
    auto orientation = orientation_for(dir);
    // TODO figure out how I'm getting duplicate edges.
    // This check shouldn't be necessary.
    if (!std::any_of(n.edges.begin(), n.edges.end(),
                     [prev](const Edge& e) { return e.dest == prev; })) {
      n.edges.push_back({&n, prev, orientation, distance});
      prev->edges.push_back({prev, &n, orientation, distance});
    }

    prev = &n;
    distance = 0;
    if (is_free_path(grid, current)) {
      grid[current.first][current.second] = 'X';
      for (auto d : ALL_DIRECTIONS) {
        if (d != opposite(dir)) {
          collect_vertexes_impl(grid, all_nodes, prev, adjust(d, current), d,
                                distance + 1);
        }
      }
    }
  } else {
    grid[current.first][current.second] = 'X';
    collect_vertexes_impl(grid, all_nodes, prev, adjust(dir, current), dir,
                          distance + 1);
  }
}

PointToNode collect_vertexes(Grid& grid) {
  auto start = Point{grid.size() - 2, 0};
  grid[start.first][start.second] = 'X';
  auto next = Point{start.first, start.second + 1};
  PointToNode all_nodes = {{start, {}}, {next, {}}};
  grid[next.first][next.second] = '.';
  collect_vertexes_impl(grid, all_nodes, &all_nodes[start], next,
                        Direction::right, 0);
  auto& start_node = all_nodes[start];
  start_node.distances[Orientation::horizontal].distance = 0;
  start_node.distances[Orientation::vertical].distance = TURN_PENALTY;
  return all_nodes;
}

NodeHeap build_heap(PointToNode& all_nodes) {
  NodeHeap heap;
  for (auto& n : std::views::values(all_nodes)) {
    heap.push(&n);
  }
  return heap;
}

DistanceHistory get_best_distance(const Node& n, const Edge& e) {
  auto without_turn = n.distances.at(e.orientation).distance;
  auto with_turn = n.distances.at(other(e.orientation)).distance;

  if (without_turn != std::numeric_limits<long>::max()) {
    without_turn += e.length;
  }
  if (with_turn != std::numeric_limits<long>::max()) {
    with_turn += e.length + TURN_PENALTY;
  }
  if (without_turn < with_turn) {
    auto paths = n.distances.at(e.orientation).edge_paths;
    for (auto& p : paths) {
      p.push_back(&e);
    }
    return {without_turn, paths};
  }
  if (with_turn < without_turn) {
    auto paths = n.distances.at(other(e.orientation)).edge_paths;
    for (auto& p : paths) {
      p.push_back(&e);
    }
    return {with_turn, paths};
  }
  assert(without_turn != std::numeric_limits<long>::max() &&
         with_turn != std::numeric_limits<long>::max());
  auto paths = n.distances.at(e.orientation).edge_paths;
  const auto& other_paths = n.distances.at(other(e.orientation)).edge_paths;
  paths.insert(paths.end(), other_paths.begin(), other_paths.end());
  for (auto& p : paths) {
    p.push_back(&e);
  }
  return {with_turn, paths};
}

void find_shortest(NodeHeap& heap, const Node* end) {
  while (!heap.empty()) {
    auto n = heap.top();
    heap.pop();
    if (n->min_distance() == std::numeric_limits<long>::max()) {
      break;
    }
    if (n == end) {
      continue;
    }
    for (const auto& e : n->edges) {
      auto dist = get_best_distance(*n, e);
      if (dist.distance < e.dest->distances[e.orientation].distance) {
        e.dest->distances[e.orientation] = std::move(dist);
        heap.decrease_key(e.dest);
      } else if (dist.distance == e.dest->distances[e.orientation].distance) {
        auto& ep = e.dest->distances[e.orientation].edge_paths;
        ep.insert(ep.end(), dist.edge_paths.begin(), dist.edge_paths.end());
      }
    }
    n->distances[Orientation::horizontal].edge_paths.clear();
    n->distances[Orientation::vertical].edge_paths.clear();
  }
}

std::size_t count_used(
    const std::unordered_set<const Edge*>& used_edges,
    const std::unordered_map<const Node*, Point>& node_to_point) {
  std::unordered_set<Point, PointHash> used_tiles;
  for (const auto* e : used_edges) {
    Point p = node_to_point.at(e->src);
    Point last = node_to_point.at(e->dest);
    if (p.first == 0 || p.second == 0 || last.first == 0 || last.second == 0) {
      continue;
    }
    if (e->orientation == Orientation::vertical) {
      if (p.first > last.first) {
        std::swap(p, last);
      }
    } else {
      if (p.second > last.second) {
        std::swap(p, last);
      }
    }
    while (p != last) {
      used_tiles.insert(p);
      if (e->orientation == Orientation::vertical) {
        ++p.first;
      } else {
        ++p.second;
      }
    }
    used_tiles.insert(last);
  }
  return used_tiles.size();
}

}  // namespace

int main() {
  Grid grid(std::istream_iterator<std::string>{std::cin},
            std::istream_iterator<std::string>{});
  auto all_nodes = collect_vertexes(grid);
  auto heap = build_heap(all_nodes);
  auto& end = all_nodes[Point{1, grid[1].size() - 2}];
  std::unordered_map<const Node*, Point> node_to_point;
  for (const auto& [p, n] : all_nodes) {
    node_to_point[&n] = p;
  }
  find_shortest(heap, &end);

  // TODO what happens if they're the same?
  auto shorter =
      std::min(end.distances[Orientation::horizontal],
               end.distances[Orientation::vertical],
               [](const DistanceHistory& lhs, const DistanceHistory& rhs) {
                 return lhs.distance < rhs.distance;
               });

  std::unordered_set<const Edge*> used_edges;
  for (const auto& path : shorter.edge_paths) {
    used_edges.insert(path.begin(), path.end());
  }
  std::cout << count_used(used_edges, node_to_point) << '\n';
}
