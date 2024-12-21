#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << ' ' ;
  });
  return out;
}

using Pair = std::pair<int, int>;

int dimensions(std::vector<Pair> positions) {
  int result = 6;
  for (auto [x, y]: positions) {
    if (x > 6) result = 70;
  }

  return result;
}

int bfs(std::vector<Pair> positions, int bytes, int n) {
  positions.resize(bytes);

  auto check = [=](int x, int y) -> bool {
    if (x < 0 || n < x) return false;
    if (y < 0 || n < y) return false;
    return true;
  };

  std::vector<std::vector<bool>> map(n + 1, std::vector<bool>(n + 1));
  std::vector<std::vector<bool>> visited(n + 1, std::vector<bool>(n + 1));

  for (auto [x, y]: positions) map[y][x] = true;

  std::vector<std::vector<int>> distances(n + 1, std::vector<int>(n + 1));
  std::deque<Pair> queue { { 0, 0 } };
  visited[0][0] = true;

  while (!queue.empty()) {
    auto [x, y] = queue.front();
    queue.pop_front();

    if (map[y][x]) continue;

    if (check(x + 1, y) && !visited[y][x + 1]) {
      visited[y][x + 1] = true;
      distances[y][x + 1] = distances[y][x] + 1;
      queue.emplace_back(x + 1, y);
    }
    if (check(x - 1, y) && !visited[y][x - 1]) {
      visited[y][x - 1] = true;
      distances[y][x - 1] = distances[y][x] + 1;
      queue.emplace_back(x - 1, y);
    }
    if (check(x, y + 1) && !visited[y + 1][x]) {
      visited[y + 1][x] = true;
      distances[y + 1][x] = distances[y][x] + 1;
      queue.emplace_back(x, y + 1);
    }
    if (check(x, y - 1) && !visited[y - 1][x]) {
      visited[y - 1][x] = true;
      distances[y - 1][x] = distances[y][x] + 1;
      queue.emplace_back(x, y - 1);
    }
  }

  return distances[n][n];
}

void first(std::vector<Pair> positions) {
  int n = dimensions(positions);
  int bytes = 12;
  if (n != 6) bytes = 1024;
  std::cout << bfs(positions, bytes, n) << nl;
}

void second(std::vector<Pair> positions) {
  int n = dimensions(positions);

  int bytes = 1;
  for (int add = n * n; add > 0; add /= 2) {
    while (bfs(positions, bytes + add, n) != 0) {
      bytes += add;
    }
  }

  auto [x, y] = positions[bytes];
  std::cout << x << ',' << y << nl;
}

int main() {
  std::vector<Pair> positions;
  std::string line;
  while (std::getline(std::cin, line)) {
    for (char& c: line) {
      if (c == ',') c = ' ';
    }
    std::istringstream stream { line };

    int x, y;
    stream >> x >> y;
    positions.emplace_back(x, y);
  }

  first(positions);
  second(positions);
}
