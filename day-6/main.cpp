#include <iostream>
#include <vector>
#include <array>


constexpr char newline { '\n' };

struct Point {
  int x, y;
};

std::vector<Point> directions {
  { 1, 0 },
  { 0, 1 },
  { -1, 0 },
  { 0, -1 }
};

void first(std::vector<std::string> lines, int x, int y ) {
  int direction = 3;

  int width = lines[0].size();
  int height = lines.size();

  std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
  while (true) {
    visited[y][x] = true;
    auto [dx, dy] = directions[direction];

    if (x+dx < 0 || x+dx >= width || y+dy < 0 || y+dy >= height) {
      break;
    }

    if (lines[y + dy][x + dx] == '#') {
      direction = (direction + 1) % 4;
      continue;
    }

    x += dx;
    y += dy;

  }

  int result = 0;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (visited[y][x]) ++result;
    }
  }

  std::cout << result << newline;
}

bool is_loop(std::vector<std::string> lines, int x, int y) {
  int width = lines[0].size();
  int height = lines.size();

  int direction = 3;
  std::vector<std::vector<std::array<bool, 4>>> visited(height, std::vector<std::array<bool, 4>>(width));
  while (true) {
    if (visited[y][x][direction]) return true;
    visited[y][x][direction] = true;

    auto [dx, dy] = directions[direction];
    if (x+dx < 0 || x+dx >= width || y+dy < 0 || y+dy >= height) {
      return false;
    }

    if (lines[y + dy][x + dx] == '#') {
      direction = (direction + 1) % 4;
      continue;
    }

    x += dx;
    y += dy;
  }
}

void second(std::vector<std::string> lines, int start_x, int start_y ) {
  int width = lines[0].size();
  int height = lines.size();

  int result = 0;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (lines[y][x] == '.') {
        lines[y][x] = '#';
        if (is_loop(lines, start_x, start_y)) ++result;
        lines[y][x] = '.';
      }
    }
  }

  std::cout << result << newline;
}

int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::cin >> line) {
    lines.push_back(line);
  }

  int width = lines[0].size();
  int height = lines.size();

  Point position;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (lines[y][x] == '^') {
        position = { x, y };
      }
    }
  }
  auto [x, y] = position;

  first(lines, x, y);
  second(lines, x, y);
}
