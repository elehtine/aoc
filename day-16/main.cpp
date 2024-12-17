#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <tuple>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << nl;
  });
  return out;
}

enum class Direction {
  north, east, south, west
};

Direction operator++(Direction& direction) {
  int dir = static_cast<int>(direction) + 1;
  direction = static_cast<Direction>(dir);
  return direction;
}

using Point = std::pair<int, int>;
using Location = std::tuple<int, int, Direction>;

std::map<Direction, Point> direction_to_point {
  { Direction::north, { 0, -1 } },
  { Direction::east, { 1, 0 } },
  { Direction::south, { 0, 1 } },
  { Direction::west, { -1, 0 } },
};


Point dimensions(std::vector<std::string> map) {
  return { map[0].size(), map.size() };
}

struct Result {
  std::map<Location, int> scores;
  std::map<Location, std::set<Location>> parents;
};

Result first(Point start, Point end, std::vector<std::string> map) {
  std::map<Location, int> scores;
  std::map<Location, std::set<Location>> parents;

  using Pair = std::pair<int, Location>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> queue;
  queue.push({ 0, { start.first, start.second, Direction::east } });

  while (!queue.empty()) {
    auto [score, location] = queue.top();
    queue.pop();

    auto [x, y, direction] = location;
    if (map[y][x] == '#') continue;
    if (score != scores[location]) continue;

    for (Direction next_dir = Direction::north; next_dir <= Direction::west; ++next_dir) {
      if (next_dir == direction) {
        auto [dx, dy] = direction_to_point[next_dir];
        Location next_location = { x + dx, y + dy, next_dir };

        if (!scores.contains(next_location) || scores[location] + 1 < scores[next_location]) {
          scores[next_location] = scores[location] + 1;
          queue.push({ scores[location] + 1, next_location });
          parents[next_location].clear();
          parents[next_location].insert(location);
        } else if (scores[location] + 1 == scores[next_location]) {
          parents[next_location].insert(location);
        }
      } else {
        Location next_location = { x, y, next_dir };

        if (!scores.contains(next_location) || scores[location] + 1000 < scores[next_location]) {
          scores[next_location] = scores[location] + 1000;
          queue.push({ scores[location] + 1000, next_location });
          parents[next_location].clear();
          parents[next_location].insert(location);
        } else if (scores[location] + 1000 == scores[next_location]) {
          parents[next_location].insert(location);
        }
      }
    }
  }

  return { scores, parents };
}

void second(std::set<Location> ends, std::map<Location, std::set<Location>> parents) {
  std::set<Point> visited;
  std::deque<Location> queue(ends.begin(), ends.end());

  while (!queue.empty()) {
    Location location = queue.front();
    queue.pop_front();

    Point point { std::get<0>(location), std::get<1>(location) };
    visited.insert(point);

    for (Location parent: parents[location]) {
      queue.push_back(parent);
    }
  }

  std::cout << visited.size() << nl;
}

int main() {
  std::string line;
  std::vector<std::string> map;
  while (std::getline(std::cin, line)) {
    map.push_back(line);
  }

  Point start;
  Point end;
  auto [width, height] = dimensions(map);
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == 'S') start = { x, y };
      if (map[y][x] == 'E') end = { x, y };
    }
  }

  auto [scores, parents] = first(start, end, map);

  std::set<Location> ends { { end.first, end.second, Direction::north } };
  for (Direction direction = Direction::north; direction <= Direction::west; ++direction) {
    Location location { end.first, end.second, direction };
    if (!scores.contains(location)) continue;
    if (scores[location] == scores[*ends.begin()]) {
      ends.insert(location);
    }
    if (scores[location] < scores[*ends.begin()]) {
      ends.clear();
      ends.insert(location);
    }
  }
  std::cout << scores[*ends.begin()] << nl;

  second(ends, parents);
}
