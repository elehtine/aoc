#include <iostream>
#include <vector>

using namespace std;

struct Number {
  int row, col, length;
};

enum class Type {
  empty, number, symbol
};

struct Symbol {
  int row, col;
  char symbol;
  vector<Number> numbers;
};

vector<string> lines;
vector<Number> engine;
vector<Symbol> symbols;
vector<int> result;

Type type(char c) {
  int digit = c - '0';
  if (0 <= digit && digit <= 9) return Type::number;
  if (c == '.') return Type::empty;
  return Type::symbol;
}

int parse(const Number& number) {
  int num = 0;
  string line = lines[number.row];
  for (int index = 0; index < number.length; index++) {
    int digit = line[number.col + index] - '0';
    num *= 10;
    num += digit;
  }
  return num;
}

ostream& operator<<(ostream& output, const Number& number) {
  int num = parse(number);
  output << num;
  return output;
}

ostream& operator<<(ostream& output, const pair<int, int>& p) {
  output << "{ ";
  output << p.first << ", " << p.second;
  output << " }";
  return output;
}

template<typename T>
ostream& operator<<(ostream& output, const vector<T>& v) {
  bool comma = false;
  output << "[ ";
  for (const T& value: v) {
    if (comma) output << ", ";
    comma = true;
    output << value;
  }
  output << " ]";
  return output;
}

void read() {
  string s;
  while (getline(cin, s)) {
    lines.push_back(s);
  }

  for (size_t row = 0; row < lines.size(); row++) {
    string line = lines[row];
    int start, current = 0;
    for (size_t col = 0; col < line.size(); col++) {
      Type t = type(line[col]);
      if (t != Type::number && current != 0) {
        engine.push_back(Number { (int) row, start, current });
        current = 0;
      }
      if (t == Type::symbol) {
        symbols.push_back(Symbol { (int) row, (int) col, line[col], {}});
      }
      if (t == Type::number) {
        if (current == 0) start = col;
        current++;
      }
    }

    if (current != 0) {
        engine.push_back(Number { (int) row, start, current });
    }
  }
}

bool check(const Number& number) {
  bool found = false;
  int width = lines[0].size();
  int height = lines.size();
  for (int drow = -1; drow <= 1; drow++) {
    for (int dcol = -1; dcol <= number.length; dcol++) {
      if (number.row + drow < 0 || number.row + drow >= height) continue;
      if (number.col + dcol < 0 || number.col + dcol >= width) continue;

      for (Symbol& symbol: symbols) {
        bool correct_row = symbol.row == number.row + drow;
        bool correct_col = symbol.col == number.col + dcol;
        if (correct_row && correct_col) {
          found = true;
          symbol.numbers.push_back(number);
        }
      }
    }
  }

  return found;
}

void solve() {
  for (const Number& number: engine) {
    if (check(number)) {
      result.push_back(parse(number));
    }
  }

  int second = 0;
  for (const Symbol& symbol: symbols) {
    if (symbol.symbol != '*') continue;
    if (symbol.numbers.size() != 2) continue;
    second += parse(symbol.numbers[0]) * parse(symbol.numbers[1]);
  }
  cout << second << endl;

  int first = 0;
  for (int num: result) first += num;
  cout << first << endl;
}

int main() {
  read();
  solve();
}
