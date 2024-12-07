#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>


constexpr char nl { '\n' };

struct Equation {
  int64_t result;
  std::vector<int64_t> numbers;
};

int64_t digits(int64_t number) {
  if (number == 0) return 0;
  return 1 + digits(number / 10);
}

int64_t power(int64_t base, int64_t exponent) {
  int64_t result = 1;
  while (exponent > 0) {
    if (exponent % 2 == 1) result *= base;
    base = base * base;
    exponent /= 2;
  }
  return result;
}

int64_t concatenate(int64_t left, int64_t right) {
  return left * power(10, digits(right)) + right;
}

int64_t first(int64_t& left, int64_t right, int64_t result) {
  int64_t product = left * right;
  if (product <= result && product / right == left) {
    return product;
  }

  return -1;
}

int64_t second(int64_t left, int64_t right, int64_t result) {
  if (digits(left) + digits(right) <= digits(result)) {
    int64_t concatenation = concatenate(left, right);
    if (concatenation <= result) {
      return concatenation;
    }
  }

  return -1;
}

int main() {
  std::vector<Equation> equations;

  std::string line;
  while (std::getline(std::cin, line)) {
    for (char& c: line) {
      if (c == ':') c = ' ';
    }

    std::istringstream stream { line };
    Equation equation;
    stream >> equation.result;

    int64_t number;
    while (stream >> number) {
      equation.numbers.push_back(number);
    }
    equations.push_back(equation);
  }


  int64_t first_total = 0;
  int64_t second_total = 0;

  for (Equation equation: equations) {
    std::vector<int64_t> first_results { 0 };
    std::vector<int64_t> second_results { 0 };

    for (int64_t number: equation.numbers) {
      int first_size = first_results.size();
      int second_size = second_results.size();

      for (int index = 0; index < first_size; ++index) {
        int64_t product = first(first_results[index], number, equation.result);
        if (product != -1) first_results.push_back(product);
        first_results[index] += number;
      }

      for (int index = 0; index < second_size; ++index) {
        int64_t product = first(second_results[index], number, equation.result);
        int64_t concatenation = second(second_results[index], number, equation.result);

        if (product != -1) second_results.push_back(product);
        if (concatenation != -1) second_results.push_back(concatenation);
        second_results[index] += number;
      }
    }

    if (std::ranges::find(first_results, equation.result) != first_results.end()) {
      first_total += equation.result;
    }
    if (std::ranges::find(second_results, equation.result) != second_results.end()) {
      second_total += equation.result;
    }
  }

  std::cout << first_total << nl;
  std::cout << second_total << nl;
}
