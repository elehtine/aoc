#include <iostream>
#include <regex>
#include <algorithm>
#include <vector>


constexpr char newline = '\n';
const std::string do_string = "do()";
const std::string dont_string = "don't()";
const std::regex mul { R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))" };


int first(std::smatch match) {
  if (match[0] == do_string || match[0] == dont_string) {
    return 0;
  }

  return std::stoi(match[1]) * std::stoi(match[2]);
}

int second(std::smatch match, bool& dont) {
  if (match[0] == do_string) {
    dont = false;
    return 0;
  }

  if (match[0] == dont_string) {
    dont = true;
    return 0;
  }

  if (!dont) {
    return std::stoi(match[1]) * std::stoi(match[2]);
  }

  return 0;
}

int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }


  int first_result = 0;
  int second_result = 0;
  bool dont = false;

  std::ranges::for_each(lines,
      [&first_result, &second_result, &dont](std::string line) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), mul);
        std::for_each(begin, std::sregex_iterator {},
            [&first_result, &second_result, &dont](std::smatch match) {
              first_result += first(match);
              second_result += second(match, dont);
            });
      });

  std::cout << first_result << newline;
  std::cout << second_result << newline;
}
