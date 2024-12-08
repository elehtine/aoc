#include <iostream>
#include <compare>
#include <algorithm>
#include <vector>
#include <set>
#include <map>


constexpr char nl { '\n' };

struct Point {
  friend std::strong_ordering operator<=>(const Point& left, const Point& right) = default;
  int x, y;
};


bool check(int x, int y, int width, int height) {
  if (x < 0 || width <= x) return false;
  if (y < 0 || height <= y) return false;
  return true;
};

void first(std::map<char, std::vector<Point>> antenna_map, int width, int height) {
  std::set<Point> result;
  for (auto [c, antennas]: antenna_map) {
    for (Point first: antennas) {
      for (Point second: antennas) {
        if (first == second) continue;
        int dx = first.x - second.x;
        int dy = first.y - second.y;

        if (check(first.x + dx, first.y + dy, width, height)) {
          result.insert({ first.x + dx, first.y + dy });
        }
        if (check(second.x - dx, second.y - dy, width, height)) {
          result.insert({ second.x - dx, second.y - dy });
        }
      }
    }
  }

  std::cout << result.size() << nl;
}

void second(std::map<char, std::vector<Point>> antenna_map, int width, int height) {
  std::set<Point> result;
  for (auto [c, antennas]: antenna_map) {
    for (Point antenna: antennas) {
      result.insert(antenna);
    }

    for (Point first: antennas) {
      for (Point second: antennas) {
        if (first == second) continue;
        int dx = first.x - second.x;
        int dy = first.y - second.y;

        for (int multiply = 1; check(first.x + multiply*dx, first.y + multiply*dy, width, height); ++multiply) {
          result.insert({ first.x + multiply*dx, first.y + multiply*dy });
        }
        for (int multiply = 1; check(second.x - multiply*dx, second.y - multiply*dy, width, height); ++multiply) {
          result.insert({ second.x - multiply*dx, second.y - multiply*dy });
        }
      }
    }
  }

  std::cout << result.size() << nl;
}

int main() {
  std::vector<std::string> lines;
  std::string line;

  while (std::cin >> line) {
    lines.push_back(line);
  }

  int width = lines[0].size();
  int height = lines.size();

  std::map<char, std::vector<Point>> antenna_map;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (lines[y][x] == '.') continue;
      antenna_map[lines[y][x]].push_back({ x, y });
    }
  }

  first(antenna_map, width, height);
  second(antenna_map, width, height);
}
