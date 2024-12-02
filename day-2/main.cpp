#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <vector>


constexpr char newline = '\n';

bool positive(auto begin, auto end, bool dropped) {
  std::vector<int> differences(std::distance(begin, end));
  std::adjacent_difference(begin, end, differences.begin());

  auto in_range = [](int number) {
    return number == std::clamp(number, 1, 3);
  };

  for (int index = 1; index < std::ssize(differences); ++index) {
    if (in_range(differences[index])) {
      continue;
    }

    if (dropped) return false;
    dropped = true;

    ++index;
    if (index == std::ssize(differences) || in_range(differences[index] + differences[index - 1])) {
      continue;
    }

    if (index == 2 || in_range(differences[index - 2] + differences[index - 1])) {
      if (in_range(differences[index])) {
        continue;
      }
    }

    return false;
  }

  return true;
}


void first(std::vector<std::vector<int>> reports) {
  int result = std::ranges::count_if(reports, [](std::vector<int> report) {
    if (positive(report.begin(), report.end(), true)) return true;
    if (positive(report.rbegin(), report.rend(), true)) return true;
    return false;
  });

  std::cout << result << newline;
}

void second(std::vector<std::vector<int>> reports) {
  int result = std::ranges::count_if(reports, [](std::vector<int> report) {
    if (positive(report.begin(), report.end(), false)) return true;
    if (positive(report.rbegin(), report.rend(), false)) return true;
    
    return false;
  });

  std::cout << result << newline;
}

int main() {
  std::vector<std::vector<int>> reports;

  std::string line;
  while (std::getline(std::cin, line)) {
    reports.push_back({});
    std::istringstream stream { line };

    int value;
    while (stream >> value) {
      reports.back().push_back(value);
    }
  }

  first(reports);
  second(reports);
}
