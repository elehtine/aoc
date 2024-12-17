#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <tuple>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << nl;
  });
  return out;
}

int64_t power(int64_t base, int64_t exponent) {
  if (exponent == 0) return 1;
  int64_t result = power(base, exponent / 2);
  result *= result;
  if (exponent % 2 == 1) result *= base;
  return result;
}

std::vector<int64_t> first(int64_t a, int64_t b, int64_t c, std::vector<int64_t> instructions) {
  std::vector<int64_t> output;
  for (int index = 0; index < std::ssize(instructions); index += 2) {
    int64_t opcode = instructions[index];
    int64_t operand = instructions[index + 1];

    int64_t combo;
    if (operand <= 3) combo = operand;
    if (operand == 4) combo = a;
    if (operand == 5) combo = b;
    if (operand == 6) combo = c;

    if (opcode == 0) a = a / power(2, combo);
    if (opcode == 1) b = b ^ operand;
    if (opcode == 2) b = combo % 8;
    if (opcode == 3 && a != 0) index = operand - 2;
    if (opcode == 4) b = b ^ c;
    if (opcode == 5) output.push_back(combo % 8);
    if (opcode == 6) b = a / power(2, combo);
    if (opcode == 7) c = a / power(2, combo);
  }

  return output;
}

bool second(int64_t a, int64_t b, int64_t c, std::vector<int64_t> instructions) {
  if (instructions.empty()) {
    std::cout << "result: " << a << nl;
    return true;
  }

  int64_t last_a = a;
  int64_t start = 0;
  if (last_a == 0) start = 1;
  for (int64_t add = start; add <= 7; ++add) {
    a = last_a * 8 + add;
    b = a % 8;
    b = b ^ 7;
    c = a / power(2, b);
    b = b ^ 7;
    b = b ^ c;
    if (b % 8 == instructions.front()) {
      if (second(a, b, c, std::vector<int64_t>(std::next(instructions.begin()), instructions.end()))) {
        return true;
      }
    }
  }

  return false;
}

int main() {
  int64_t a;
  int64_t b;
  int64_t c;

  std::vector<int64_t> instructions;

  for (int index = 0; index < 5; ++index) {
    std::string line;
    std::getline(std::cin, line);

    if (index < 3) {
      line = line.substr(12);
      if (index == 0) a = std::stoi(line);
      if (index == 1) b = std::stoi(line);
      if (index == 2) c = std::stoi(line);
    }

    if (index == 4) {
      line = line.substr(9);
      for (char& c: line) {
        if (c == ',') c = ' ';
      }

      std::istringstream stream { line };
      int64_t opcode, operand;
      while (stream >> opcode >> operand) {
        instructions.push_back(opcode);
        instructions.push_back(operand);
      }
    }
  }

  std::vector<int64_t> output = first(a, b, c, instructions);
  for (int64_t out: output) {
    std::cout << out << ',';
  }
  std::cout << nl;

  std::ranges::reverse(instructions);
  second(0, b, c, instructions);
}
