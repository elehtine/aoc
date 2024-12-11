#include <iostream>
#include <algorithm>
#include <vector>
#include <set>


constexpr char nl { '\n' };

void first(std::vector<std::vector<int>> map) {
   int width = map[0].size();
  int height = map.size();

  std::vector<std::vector<std::set<std::pair<int, int>>>> dp(height,
      std::vector<std::set<std::pair<int, int>>>(width));

  auto check = [=](int x, int y) {
    if (x < 0 || width <= x) return false;
    if (y < 0 || height <= y) return false;
    return true;
  };

  int result = 0;
  for (int h = 0; h < 10; ++h) {
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        if (map[y][x] != h) continue;
        if (h == 0) {
          dp[y][x].insert({ x, y });
          continue;
        }

        if (check(x+1, y) && map[y][x+1] == h-1) {
          for (auto pair: dp[y][x+1]) {
            dp[y][x].insert(pair);
          }
        }
        if (check(x-1, y) && map[y][x-1] == h-1) {
          for (auto pair: dp[y][x-1]) {
            dp[y][x].insert(pair);
          }
        }
        if (check(x, y+1) && map[y+1][x] == h-1) {
          for (auto pair: dp[y+1][x]) {
            dp[y][x].insert(pair);
          }
        }
        if (check(x, y-1) && map[y-1][x] == h-1) {
          for (auto pair: dp[y-1][x]) {
            dp[y][x].insert(pair);
          }
        }

        if (h == 9) result += dp[y][x].size();
      }
    }
  }

  std::cout << result << nl;
}

void second(std::vector<std::vector<int>> map) {
  int width = map[0].size();
  int height = map.size();

  std::vector<std::vector<int>> dp(height, std::vector<int>(width));

  auto check = [=](int x, int y) {
    if (x < 0 || width <= x) return false;
    if (y < 0 || height <= y) return false;
    return true;
  };

  int result = 0;
  for (int h = 0; h < 10; ++h) {
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        if (map[y][x] != h) continue;
        if (h == 0) {
          dp[y][x] = 1;
          continue;
        }

        if (check(x+1, y) && map[y][x+1] == h-1) {
          dp[y][x] += dp[y][x+1];
        }
        if (check(x-1, y) && map[y][x-1] == h-1) {
          dp[y][x] += dp[y][x-1];
        }
        if (check(x, y+1) && map[y+1][x] == h-1) {
          dp[y][x] += dp[y+1][x];
        }
        if (check(x, y-1) && map[y-1][x] == h-1) {
          dp[y][x] += dp[y-1][x];
        }

        if (h == 9) result += dp[y][x];
      }
    }
  }

  std::cout << result << nl;
}

int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }

  int width = lines[0].size();
  int height = lines.size();
  std::vector<std::vector<int>> map(height, std::vector<int>(width));

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      map[y][x] = lines[y][x] - '0';
    }
  }

  first(map);
  second(map);
}
