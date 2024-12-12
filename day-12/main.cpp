#include <iostream>
#include <vector>
#include <map>


constexpr char nl { '\n' };

class UnionFind {
  public:
    UnionFind(int width, int height): width(width), height(height) {
      for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
          parents[std::make_pair(x, y)] = { x, y };
          sizes[std::make_pair(x, y)] = 1;
        }
      }
    }

    std::pair<int, int> find(std::pair<int, int> value) {
      while (parents[value] != value) {
        parents[value] = parents[parents[value]];
        value = parents[value];
      }
      return value;
    }

    void merge(std::pair<int, int> left, std::pair<int, int> right) {
      left = find(left);
      right = find(right);

      if (sizes[left] < sizes[right]) std::swap(left, right);
      parents[right] = left;
      sizes[left] += sizes[right];
    }

  private:
    const int width;
    const int height;

    std::map<std::pair<int, int>, std::pair<int, int>> parents;
    std::map<std::pair<int, int>, int> sizes;
};


bool check(int x, int y, int width, int height) {
  if (x < 0 || width <= x) return false;
  if (y < 0 || height <= y) return false;
  return true;
}

void first(std::vector<std::string> garden, UnionFind union_find) {
  int width = garden[0].size();
  int height = garden.size();

  std::map<std::pair<int, int>, int> areas;
  std::map<std::pair<int, int>, int> inner;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      std::pair<int, int> parent = union_find.find({ x, y });
      ++areas[parent];

      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          if (std::abs(dx + dy) != 1) continue;
          if (!check(x + dx, y + dy, width, height)) continue;

          if (parent == union_find.find({ x + dx, y + dy })) {
            ++inner[parent];
          }
        }
      }
    }
  }

  int result = 0;
  for (auto [parent, area]: areas) {
    result += area * (4*area - inner[parent]);
  }
  std::cout << result << nl;
}

void second(std::vector<std::string> garden, UnionFind union_find) {
  int width = garden[0].size();
  int height = garden.size();

  std::map<std::pair<int, int>, int> areas;
  std::map<std::pair<int, int>, int> sides;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      std::pair<int, int> parent = union_find.find({ x, y });
      ++areas[parent];
    }
  }

  for (int x = 0; x <= width; ++x) {
    std::pair<int, int> left { -1, -1 };
    std::pair<int, int> right { -1, -1 };
    for (int y = 0; y < height; ++y) {
      std::pair<int, int> new_left = { -1, -1 };
      std::pair<int, int> new_right = { -1, -1 };
      if (check(x-1, y, width, height)) new_left = union_find.find({ x-1, y });
      if (check(x, y, width, height)) new_right = union_find.find({ x, y });

      if (new_left == new_right) {
        left = new_left;
        right = new_right;
        continue;
      }

      if (left != new_left) ++sides[new_left];
      else if (right == new_left) ++sides[new_left];
      if (right != new_right) ++sides[new_right];
      else if (left == new_right) ++sides[new_right];
      left = new_left;
      right = new_right;
    }
  }

  for (int y = 0; y <= height; ++y) {
    std::pair<int, int> up { -1, -1 };
    std::pair<int, int> down { -1, -1 };
    for (int x = 0; x < width; ++x) {
      std::pair<int, int> new_up = { -1, -1 };
      std::pair<int, int> new_down = { -1, -1 };
      if (check(x, y-1, width, height)) new_up = union_find.find({ x, y-1 });
      if (check(x, y, width, height)) new_down = union_find.find({ x, y });

      if (new_up == new_down) {
        up = new_up;
        down = new_down;
        continue;
      }

      if (up != new_up) ++sides[new_up];
      else if (down == new_up) ++sides[new_up];
      if (down != new_down) ++sides[new_down];
      else if (up == new_down) ++sides[new_down];
      up = new_up;
      down = new_down;
    }
  }

  int result = 0;
  for (auto [parent, area]: areas) {
    result += area * sides[parent];
  }
  std::cout << result << nl;
}

int main() {
  std::vector<std::string> garden;
  std::string line;

  while (std::getline(std::cin, line)) {
    garden.push_back(line);
  }

  int width = garden[0].size();
  int height = garden.size();

  UnionFind union_find { width, height };

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          if (std::abs(dx + dy) != 1) continue;
          if (!check(x + dx, y + dy, width, height)) continue;

          if (garden[y][x] == garden[y + dy][x + dx]) {
            union_find.merge({ x, y }, { x + dx, y + dy });
          }
        }
      }
    }
  }


  first(garden, union_find);
  second(garden, union_find);
}
