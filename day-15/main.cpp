#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << nl;
  });
  return out;
}

using Point = std::pair<int, int>;
constexpr Point fail = { -1, -1 };

enum class Direction {
  north,
  east,
  south,
  west,
};

std::map<char, Direction> char_to_direction {
  { '^', Direction::north },
  { '>', Direction::east },
  { 'v', Direction::south },
  { '<', Direction::west },
};

std::map<Direction, Point> direction_to_delta {
  { Direction::north, { 0, -1 } },
  { Direction::east, { 1, 0 } },
  { Direction::south, { 0, 1 } },
  { Direction::west, { -1, 0 } },
};

bool check(const std::vector<std::string> map, Point robot, Direction direction) {
  auto [dx, dy] = direction_to_delta[direction];

  std::set<Point> current { robot };
  std::set<Point> next;
  while (!current.empty()) {

    for (auto [x, y]: current) {
      char tile = map[y][x];
      if (tile == '#') return false;
      if (tile == '@' || tile == 'O' || tile == '[' || tile == ']') {
        next.insert({ x+dx, y+dy });
      }
      if (direction == Direction::north || direction == Direction::south) {
        if (tile == '[') next.insert({ x+dx+1, y+dy });
        if (tile == ']') next.insert({ x+dx-1, y+dy });
      }
    }

    current = next;
    next.clear();
  }

  return true;
}

void first(std::vector<std::string> map, std::string instructions) {
  int width = map[0].size();
  int height = map.size();

  Point robot;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == '@') robot = { x, y };
    }
  }

  for (char instruction: instructions) {
    Direction direction = char_to_direction[instruction];
    if (!check(map, robot, direction)) continue;

    auto [dx, dy] = direction_to_delta[direction];
    for (int steps = 1; true; ++steps) {
      auto [x, y] = robot;
      if (map[y + steps*dy][x + steps*dx] == '.') {
        map[y + steps*dy][x + steps*dx] = map[y + dy][x + dx];
        map[y][x] = '.';
        map[y + dy][x + dx] = '@';
        robot = { x + dx, y + dy };
        break;
      }
    }
  }

  int result = 0;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == 'O') result += 100*y + x;
    }
  }
  std::cout << result << nl;
}

void second(std::vector<std::string> map, std::string instructions) {
  int width = map[0].size();
  int height = map.size();

  Point robot;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == '@') robot = { x, y };
    }
  }

  for (char instruction: instructions) {
    Direction direction = char_to_direction[instruction];
    auto [dx, dy] = direction_to_delta[direction];
    if (!check(map, robot, direction)) continue;

    map[robot.second][robot.first] = '.';

    std::map<Point, char> current;
    std::map<Point, char> last { { robot, '@' } };

    robot.first += dx;
    robot.second += dy;

    while (!last.empty()) {
      for (auto [point, c]: last) {
        auto [x, y] = point;
        char tile = map[y + dy][x + dx];
        if (tile == '.') continue;
        current[std::make_pair(x+dx, y+dy)] = tile;

        if (direction == Direction::north || direction == Direction::south) {
          if (tile == '[') {
            current[std::make_pair(x+dx+1, y+dy)] = map[y + dy][x + dx + 1];
          }
          if (tile == ']') {
            current[std::make_pair(x+dx-1, y+dy)] = map[y + dy][x + dx - 1];
          }
        }
      }

      for (auto [point, c]: current) {
        map[point.second][point.first] = '.';
      }

      for (auto [point, c]: last) {
        auto [x, y] = point;
        map[y + dy][x + dx] = c;
      }

      last = current;
      current.clear();
    }
  }

  int result = 0;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == '[') result += 100*y + x;
    }
  }
  std::cout << result << nl;
}

int main() {
  std::vector<std::string> map;
  std::string line;

  std::getline(std::cin, line);
  while (!line.empty()) {
    map.push_back(line);
    std::getline(std::cin, line);
  }


  std::string instructions;
  while (std::getline(std::cin, line)) {
    instructions += line;
  }

  first(map, instructions);

  std::vector<std::string> large_map(map.size());
  for (int index = 0; index < std::ssize(map); ++index) {
    for (char c: map[index]) {
      if (c == '#') large_map[index] += "##";
      if (c == 'O') large_map[index] += "[]";
      if (c == '.') large_map[index] += "..";
      if (c == '@') large_map[index] += "@.";
    }
  }

  second(large_map, instructions);
}
