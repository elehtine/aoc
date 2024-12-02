#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>


constexpr char newline = '\n';

void first(std::vector<int> left, std::vector<int> right) {
  std::ranges::sort(left);
  std::ranges::sort(right);

  int result = 0;
  for (int index = 0; index < std::ssize(left); ++index) {
    result += std::abs(left[index] - right[index]);
  }
  std::cout << result << newline;
}

void second(std::vector<int> left, std::vector<int> right) {
  std::map<int, int> occurences;
  std::ranges::for_each(right, [&occurences](int value) {
    ++occurences[value];
  });

  int result = 0;
  for (int l: left) {
    result += l * occurences[l];
  }
  std::cout << result << newline;
}

int main() {
  std::vector<int> left;
  std::vector<int> right;

  int l, r;
  while (std::cin >> l >> r) {
    left.push_back(l);
    right.push_back(r);
  }

  first(left, right);
  second(left, right);
}
