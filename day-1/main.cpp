#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> lines;
vector<int> numbers;

vector<pair<string, int>> digits {
  { "zero", 0 },
  { "one", 1 },
  { "two", 2 },
  { "three", 3 },
  { "four", 4 },
  { "five", 5 },
  { "six", 6 },
  { "seven", 7 },
  { "eight", 8 },
  { "nine", 9 },
};


void read() {
  string s;
  while (getline(cin, s)) {
    lines.push_back(s);
  }
}

int parse(const string& s, int index) {
  char c = s[index];
  int value = c - '0';
  if (0 <= value && value <= 9) {
    return value;
  }

  for (pair<string, int> digit: digits) {
    string name = digit.first;
    int number = digit.second;
    if (index + name.size() > s.size()) continue;
    if (s.substr(index, name.size()) == name) {
      return number;
    }
  }

  return -1;
}

void solve() {
  for (const string& line: lines) {
    int first = 0, last = 0;
    for (size_t index = 0; index < line.size(); index++) {
      int result = parse(line, index);
      if (result != -1) {
        first = result;
        break;
      }
    }

    for (size_t index = line.size() - 1; index >= 0; index--) {
      int result = parse(line, index);
      if (result != -1) {
        last = result;
        break;
      }
    }

    numbers.push_back(first * 10 + last);
  }

  int sum = 0;
  for (int number: numbers) {
    sum += number;
  }
  cout << sum << endl;
}

int main() {
  read();
  solve();
}
