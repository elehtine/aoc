#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>


constexpr char nl { '\n' };

int digits(int64_t stone) {
  return std::to_string(stone).size();
}

std::pair<int64_t, int64_t> half(int64_t stone) {
  std::string text = std::to_string(stone);
  int64_t left = std::stoi(text.substr(0, text.size() / 2));
  int64_t right = std::stoi(text.substr(text.size() / 2));
  return { left, right };
}

void first(std::vector<int64_t> stones) {
  std::vector<int64_t> next;

  for (int index = 0; index < 25; ++index) {
    for (int64_t stone: stones) {
      if (stone == 0) {
        next.push_back(1);
      } else if (digits(stone) % 2 == 0) {
        auto [left, right] = half(stone);
        next.push_back(left);
        next.push_back(right);
      } else {
        next.push_back(stone * 2024);
      }
    }

    stones = next;
    next.clear();
  }

  std::cout << stones.size() << nl;
}

void second(std::vector<int64_t> input) {
  std::map<int64_t, int64_t> stones;
  for (int64_t stone: input) {
    ++stones[stone];
  }
  std::map<int64_t, int64_t> next;

  for (int index = 0; index < 75; ++index) {
    for (auto [stone, count]: stones) {
      if (stone == 0) {
        next[1] += count;
      } else if (digits(stone) % 2 == 0) {
        auto [left, right] = half(stone);
        next[left] += count;
        next[right] += count;
      } else {
        next[stone * 2024L] += count;
      }
    }

    stones = next;
    next.clear();
  }

  int64_t result = 0;
  for (auto [stone, count]: stones) {
    result += count;
  }
  std::cout << result << nl;
}

int main() {
  std::vector<int64_t> stones;
  int64_t stone;

  while (std::cin >> stone) {
    stones.push_back(stone);
  }

  first(stones);
  second(stones);
}
