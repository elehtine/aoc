#include <iostream>
#include <utility>
#include <vector>
#include <set>


constexpr char newline { '\n' };

const std::string xmas { "XMAS" };
std::vector<std::pair<int, int>> directions {
  { 1, 0 },
  { 1, 1 },
  { 0, 1 },
  { -1, 1 },
  { -1, 0 },
  { -1, -1 },
  { 0, -1 },
  { 1, -1 },
};
std::vector<std::pair<int, int>> x_directions {
  { 1, 1 },
  { 1, -1 },
};

int first(std::vector<std::string> lines) {
  int result = 0;

  int width = lines[0].size();
  int height = lines.size();
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      for (auto [dx, dy]: directions) {
        if (x < 0 || width <= x || x + 3*dx < 0 || width <= x + 3*dx) {
          continue;
        }
        if (y < 0 || height <= y || y + 3*dy < 0 || height <= y + 3*dy) {
          continue;
        }

        bool match = true;
        for (int diff = 0; diff < 4; ++diff) {
          if (lines[y + dy*diff][x + dx*diff] != xmas[diff]) {
            match = false;
          }
        }

        if (match) ++result;
      }
    }
  }

  return result;
}

int second(std::vector<std::string> lines) {
  int result = 0;

  int width = lines[0].size();
  int height = lines.size();
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (x-1 < 0 || width <= x+1) {
        continue;
      }
      if (y-1 < 0 || height <= y+1) {
        continue;
      }

      if (lines[y][x] != 'A') {
        continue;
      }

      bool match = true;
      for (auto [dx, dy]: x_directions) {
        std::set<char> found;
        found.insert(lines[y + dy][x + dx]);
        found.insert(lines[y - dy][x - dx]);
        if (!found.contains('M') || !found.contains('S')) {
          match = false;
        }
      }

      if (match) ++result;
    }
  }

  return result;
}

int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }

  std::cout << first(lines) << newline;
  std::cout << second(lines) << newline;
}
