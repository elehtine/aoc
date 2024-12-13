#include <iostream>
#include <numeric>
#include <regex>
#include <vector>


constexpr char nl { '\n' };
constexpr int64_t diff = 10'000'000'000'000;

std::regex button_regex { R"(Button (A|B): X\+(\d+), Y\+(\d+))" };
std::regex prize_regex { R"(Prize: X=(\d+), Y=(\d+))" };

using Point = std::pair<int64_t, int64_t>;

struct Machine {
  Point a_button;
  Point b_button;
  Point prize;
};

// a1*x + b1*y = c1
// a2*x + b2*y = c2
//
// a1*x = c1 - b1*y 
// (a1*b2 - a2*b1)*y = a1*c2 - a2*c1 

void first(std::vector<Machine> machines) {
  int64_t tokens = 0;
  for (auto [a_button, b_button, prize]: machines) {
    int64_t a1 = a_button.first;
    int64_t b1 = b_button.first;
    int64_t c1 = prize.first;
    int64_t a2 = a_button.second;
    int64_t b2 = b_button.second;
    int64_t c2 = prize.second;
    for (int64_t second = 0; second <= 100; ++second) {
      if ((a1*b2 - a2*b1) * second == a1*c2 - a2*c1) {
        if ((c1 - b1*second) % a1 == 0) {
          int64_t first = (c1 - b1*second) / a1;
          tokens += second + 3*first;
        }
      }
    }
  }

  std::cout << tokens << nl;
}

void second(std::vector<Machine> machines) {
  int64_t tokens = 0;
  for (auto [a_button, b_button, prize]: machines) {
    prize.first += diff;
    prize.second += diff;

    int64_t a1 = a_button.first;
    int64_t b1 = b_button.first;
    int64_t c1 = prize.first;
    int64_t a2 = a_button.second;
    int64_t b2 = b_button.second;
    int64_t c2 = prize.second;

    if ((a1*c2 - a2*c1) % (a1*b2 - a2*b1) == 0) {
      int64_t second = (a1*c2 - a2*c1) / (a1*b2 - a2*b1);
      if ((c1 - b1*second) % a1 == 0) {
        int64_t first = (c1 - b1*second) / a1;
        tokens += second + 3*first;
      }
    }
  }

  std::cout << tokens << nl;
}

int main() {
  std::string line;
  std::smatch match;
  Machine machine;

  std::vector<Machine> machines;
  while (std::getline(std::cin, line)) {
    std::regex_match(line, match, button_regex);
    machine.a_button = { std::stoi(match[2]), std::stoi(match[3]) };

    std::getline(std::cin, line);
    std::regex_match(line, match, button_regex);
    machine.b_button = { std::stoi(match[2]), std::stoi(match[3]) };

    std::getline(std::cin, line);
    std::regex_match(line, match, prize_regex);
    machine.prize = { std::stoi(match[1]), std::stoi(match[2]) };

    machines.push_back(machine);
    std::getline(std::cin, line);
  }

  first(machines);
  second(machines);
}
