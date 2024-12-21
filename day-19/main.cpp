#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}


int64_t match(std::string design, std::vector<std::string> towels) {
  int n = design.size();
  std::vector<int64_t> possible(n + 1);
  possible[0] = 1;

  for (int index = 0; index < n; ++index) {
    if (possible[index] == 0) continue;
    auto begin = std::next(design.begin(), index);

    for (std::string towel: towels) {
      auto result = std::mismatch(towel.begin(), towel.end(), begin, design.end());
      if (result.first == towel.end()) {
        possible[index + towel.size()] += possible[index];
      }
    }
  }

  return possible[n];
}


void first(std::vector<int64_t> counts) {
  int64_t result = 0;
  for (int64_t count: counts) {
    if (count != 0) ++result;
  }
  std::cout << result << nl;
}

void second(std::vector<int64_t> counts) {
  int64_t result = 0;
  for (int64_t count: counts) {
    result += count;
  }
  std::cout << result << nl;
}


int main() {
  std::vector<std::string> towels;

  std::string line;
  std::getline(std::cin, line);

  auto remove_comma = [](char& c) { if (c == ',') c = ' '; };
  std::ranges::for_each(line, remove_comma);

  std::istringstream stream { line };
  std::string towel;
  while (stream >> towel) {
    towels.push_back(towel);
  }

  std::vector<std::string> designs;

  std::getline(std::cin, line);
  while (std::getline(std::cin, line)) {
    designs.push_back(line);
  }

  std::vector<int64_t> counts;
  for (std::string design: designs) {
    counts.push_back(match(design, towels));
  }

  first(counts);
  second(counts);
}
