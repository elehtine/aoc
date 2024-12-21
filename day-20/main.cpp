#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}

using Pair = std::pair<int, int>;

std::pair<Pair, Pair> start_end(std::vector<std::string> map) {
  int width = map[0].size();
  int height = map.size();

  Pair start;
  Pair end;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == 'S') start = { x, y };
      if (map[y][x] == 'E') end = { x, y };
    }
  }

  return { start, end };
}

auto compute_check(int width, int height) {
  return [=](int x, int y) {
    if (x < 0 || x >= width) return false;
    if (y < 0 || y >= height) return false;
    return true;
  };
}

std::vector<Pair> directions {
  { 1, 0 },
  { -1, 0 },
  { 0, 1 },
  { 0, -1 },
};

std::vector<std::vector<int>> bfs(const std::vector<std::string>& map, Pair start) {
  int width = map[0].size();
  int height = map.size();

  auto check = compute_check(width, height);

  std::vector<std::vector<int>> distances(height, std::vector<int>(height));
  std::vector<std::vector<bool>> visited(height, std::vector<bool>(height));
  visited[start.second][start.first] = true;
  std::deque<Pair> queue { start };
  while (!queue.empty()) {
    auto [x, y] = queue.front();
    queue.pop_front();

    if (map[y][x] == '#') continue;

    for (auto [dx, dy]: directions) {
      if (check(x + dx, y + dy) && !visited[y + dy][x + dx]) {
        distances[y + dy][x + dx] = distances[y][x] + 1;
        visited[y + dy][x + dx] = true;
        queue.emplace_back(x + dx, y + dy);
      }
    }
  }

  return distances;
}

void first(const std::vector<std::string>& map) {
  int width = map[0].size();
  int height = map.size();

  auto check = compute_check(width, height);

  auto [start, end] = start_end(map);
  auto start_distances = bfs(map, start);
  auto end_distances = bfs(map, end);

  int normal = start_distances[end.second][end.first];
  std::map<int, int> cheats;

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] != '#') continue;

      for (auto [dx, dy]: directions) {
        if (!check(x + dx, y + dy) || !check(x - dx, y - dy)) continue;
        if (map[y + dy][x + dx] == '#' || map[y - dy][x - dx] == '#') continue;

        int cheat = normal;
        cheat -= start_distances[y + dy][x + dx] + end_distances[y - dy][x - dx] + 2;
        if (cheat <= 0) continue;
        ++cheats[cheat];
      }
    }
  }

  int result = 0;
  for (auto [cheat, count]: cheats) {
    if (cheat >= 100) result += count;
  }
  std::cout << result << nl;
}

void second(const std::vector<std::string>& map) {
  int width = map[0].size();
  int height = map.size();

  auto check = compute_check(width, height);

  auto [start, end] = start_end(map);
  auto start_distances = bfs(map, start);
  auto end_distances = bfs(map, end);

  int normal = start_distances[end.second][end.first];
  std::map<int, int> cheats;

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (map[y][x] == '#') continue;
      for (int dx = -20; dx <= 20; ++dx) {
        for (int dy = -20; dy <= 20; ++dy) {
          if (std::abs(dx) + std::abs(dy) > 20) continue;
          int next_x = x + dx;
          int next_y = y + dy;

          if (!check(next_x, next_y)) continue;
          if (map[next_y][next_x] == '#') continue;

          int cheat = normal;
          cheat -= start_distances[y][x] + end_distances[next_y][next_x];
          cheat -= std::abs(dx) + std::abs(dy);

          if (cheat <= 0) continue;
          ++cheats[cheat];
        }
      }
    }
  }

  int result = 0;
  for (auto [cheat, count]: cheats) {
    if (cheat >= 100) result += count;
  }
  std::cout << result << nl;
}


int main() {
  std::vector<std::string> map;
  std::string line;
  while (std::getline(std::cin, line)) {
    map.push_back(line);
  }

  first(map);
  second(map);
}
