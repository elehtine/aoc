#include <iostream>
#include <regex>
#include <algorithm>
#include <vector>
#include <map>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}

int ctoi(char c) {
  return c - '0';
}

enum class Operator {
  and_operator, or_operator, xor_operator,
};

std::map<std::string, Operator> string_to_operator {
  { "AND", Operator::and_operator },
  { "OR", Operator::or_operator },
  { "XOR", Operator::xor_operator },
};

std::map<Operator, std::string> operator_to_string {
  { Operator::and_operator, "AND" },
  { Operator::or_operator, "OR" },
  { Operator::xor_operator, "XOR" },
};

struct Gate {
  std::string left, right, output;
  Operator op;
};

Gate parse(std::string line) {
  std::regex gate_regex { R"((\w{3}) (AND|OR|XOR) (\w{3}) -> (\w{3}))" };
  std::smatch match;
  std::regex_match(line, match, gate_regex);

  return { match[1], match[3], match[4], string_to_operator[match[2]] };
}

using Pair = std::pair<std::string, std::string>;

bool evaluate(std::string variable, std::map<std::string, bool>& variables,
    std::map<std::string, Gate> gates) {
  if (variables.contains(variable)) return variables[variable];
  auto [left, right, output, op] = gates[variable];
  bool l = evaluate(left, variables, gates);
  bool r = evaluate(right, variables, gates);
  bool result;
  if (op == Operator::and_operator) result = l & r;
  if (op == Operator::or_operator) result = l | r;
  if (op == Operator::xor_operator) result = l ^ r;
  variables[output] = result;
  return result;
}

void first(std::map<std::string, bool> variables, std::map<std::string, Gate> gates) {
  std::vector<std::string> results;
  for (auto [output,  gate]: gates) {
    if (output[0] == 'z') results.push_back(output);
  }

  uint64_t number = 0;
  std::ranges::sort(results, std::greater {});
  for (std::string result: results) {
    number <<= 1;
    uint64_t current = evaluate(result, variables, gates);
    number += current;
  }
  std::cout << number << nl;
}

void second(std::map<std::string, Gate> gates) {
  for (int index = 1; index <= 44; ++index) {
    std::string output = std::to_string(index);
    if (output.size() == 1) output = "0" + output;
    output = "z" + output;

    Gate gate = gates[output];
    if (gate.op != Operator::xor_operator) {
      std::cout << gate.left << ' ' << operator_to_string[gate.op] << ' ' << gate.right << " -> " << output << nl;
    }
  }

  std::vector<std::string> carries;
  for (auto [output, gate]: gates) {
    auto [left, right, out, op] = gate;
    if (!gates.contains(left) || !gates.contains(right)) continue;

    Operator and_op = Operator::and_operator;
    Gate l = gates[left];
    Gate r = gates[right];
    if (op == Operator::or_operator) {
      carries.push_back(output);
      if (l.op != and_op || r.op != and_op) {
        std::string l_op = operator_to_string[l.op];
        std::string r_op = operator_to_string[r.op];
        std::cout << l.left << ' ' << l_op << ' ' << l.right << ", ";
        std::cout << r.left << ' ' << r_op << ' ' << r.right << " -> ";
        std::cout << operator_to_string[op] << ' ' << out << nl;
      }
    }
  }

  std::cout << carries.size() << nl;

  std::vector<std::string> result;
  result.push_back("z11");
  result.push_back("wpd");
  result.push_back("z19");
  result.push_back("mdd");
  result.push_back("z37");
  result.push_back("wts");
  result.push_back("skh");
  result.push_back("jqf");
  std::ranges::sort(result);

  for (std::string out: result) {
    std::cout << "," << out;
  }
  std::cout << nl;
}


int main() {
  std::string line;
  std::getline(std::cin, line);

  std::map<std::string, bool> variables;
  while (line != "") {
    variables[line.substr(0, 3)] = ctoi(line[5]);
    std::getline(std::cin, line);
  }

  std::map<std::string, Gate> gates;
  while (std::getline(std::cin, line)) {
    Gate gate = parse(line);
    gates[gate.output] = gate;
  }

  first(variables, gates);
  second(gates);
}
