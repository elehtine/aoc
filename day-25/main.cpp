#include <iostream>
#include <algorithm>
#include <vector>
#include <array>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}

constexpr int n = 5;

using Array = std::array<int, n>;

void first(std::vector<Array> locks, std::vector<Array> keys) {
  int result = 0;
  for (Array lock: locks) {
    for (Array key: keys) {
      bool success = true;

      for (int index = 0; index < n; ++index) {
        if (lock[index] + key[index] > 5) {
          success = false;
        }
      }

      if (success) ++result;
    }
  }

  std::cout << result;
}

void second() {
}


int main() {
  std::vector<Array> locks;
  std::vector<Array> keys;

  while (std::cin) {
    std::vector<std::string> lines(7);
    for (int index = 0; index < 7; ++index) {
      std::cin >> lines[index];
    }

    Array current { -1, -1, -1, -1, -1 };
    for (int row = 0; row < 7; ++row) {
      for (int column = 0; column < n; ++column) {
        if (lines[row][column] == '#') {
          ++current[column];
        }
      }
    }

    if (lines[0] == "#####") {
      locks.push_back(current);
    } else {
      keys.push_back(current);
    }

    std::string empty;
    std::getline(std::cin, empty);
    std::getline(std::cin, empty);
  }

  first(locks, keys);
}
