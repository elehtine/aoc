#include <iostream>
#include <regex>
#include <algorithm>
#include <vector>


constexpr char newline = '\n';


void first(std::vector<std::string> lines) {
  const std::regex mul { R"(mul\((\d+),(\d+)\))" };

  int result = 0;

  std::ranges::for_each(lines, [mul, &result](std::string line) {
    auto begin = std::sregex_iterator(line.begin(), line.end(), mul);
    auto end = std::sregex_iterator {};
    for (auto iterator = begin; iterator != end; ++iterator) {
      std::smatch match = *iterator;
      result += std::stoi(match[1]) * std::stoi(match[2]);
    }
  });

  std::cout << result << newline;
}

void second(std::vector<std::vector<int>> reports) {
}

int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }

  first(lines);
}
