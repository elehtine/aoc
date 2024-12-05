#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include <set>


constexpr char newline { '\n' };

bool is_bad(std::map<int, std::vector<int>> order, std::vector<int> numbers) {
  std::set<int> bad;
  return std::ranges::any_of(numbers, [&order, &bad](int num) {
    std::ranges::for_each(order[num], [&bad](int before) {
      bad.insert(before);
    });
    return bad.contains(num);
  });
}

void first(std::map<int, std::vector<int>> order, std::vector<std::string> updates) {
  int result = 0;
  std::ranges::for_each(updates, [&result, &order](std::string line) {
    for (char& c: line) {
      if (c == ',') c = ' ';
    }

    std::vector<int> numbers;
    int num;

    std::istringstream stream { line };
    while (stream >> num) {
      numbers.push_back(num);
    }

    if (is_bad(order, numbers)) return;

    result += numbers[numbers.size() / 2];
  });

  std::cout << result << newline;
}

void second(std::map<int, std::vector<int>> order, std::vector<std::string> updates) {
  int result = 0;
  std::ranges::for_each(updates, [&result, &order](std::string line) {
    for (char& c: line) {
      if (c == ',') c = ' ';
    }

    std::vector<int> numbers;
    int num;

    std::istringstream stream { line };
    while (stream >> num) {
      numbers.push_back(num);
    }

    if (!is_bad(order, numbers)) return;
    std::ranges::sort(numbers, [&order](int left, int right) {
      return std::ranges::find(order[right], left) != order[right].end();
    });
    result += numbers[numbers.size() / 2];
  });

  std::cout << result << newline;
}

int main() {
  std::map<int, std::vector<int>> order;

  std::string line;
  while (std::cin >> line) {
    if (line[2] != '|') break;

    int first, second;
    char delimeter;

    std::istringstream stream { line };
    stream >> first >> delimeter >> second;
    order[second].push_back(first);
  }

  std::vector<std::string> updates { line };
  while (std::cin >> line) {
    updates.push_back(line);
  }

  first(order, updates);
  second(order, updates);
}
