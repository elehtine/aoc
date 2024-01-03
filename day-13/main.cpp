#include <iostream>

#include "readwrite.h"

using namespace std;

vector<vector<string>> patterns;

void read() {
  vector<string> lines = read_lines();
  vector<string> current;
  for (string line: lines) {
    if (line == "") {
      patterns.push_back(current);
      current = vector<string>();
    } else {
      current.push_back(line);
    }
  }

  patterns.push_back(current);
  current = vector<string>();
}

constexpr int COL = 0;
constexpr int ROW = 1;

string take(vector<string> pattern, int index, int direction) {
  if (direction == ROW) {
    return pattern[index];
  }

  string result = "";
  for (int i = 0; i < (int) pattern.size(); i++) {
    result += pattern[i][index];
  }
  return result;
}

bool is_mirror(string one, int index) {
  int first = index;
  int second = index+1;
  while (first >= 0 && second < (int) one.size()) {
    if (one[first] != one[second]) return false;
    first--;
    second++;
  }
  return true;
}

int solve(vector<string> pattern, int ignore) {
  int first = 0;
  for (int row = 0; row < (int) pattern.size()-1; row++) {
    bool correct = true;
    for (int col = 0; col < (int) pattern[0].size(); col++) {
      if (!is_mirror(take(pattern, col, COL), row)) correct = false;
    }
    if (correct) {
      if ((row+1) * 100 == ignore) continue;
      first += (row+1) * 100;
    }
  }

  for (int col = 0; col < (int) pattern[0].size()-1; col++) {
    bool correct = true;
    for (int row = 0; row < (int) pattern.size(); row++) {
      if (!is_mirror(take(pattern, row, ROW), col)) correct = false;
    }
    if (correct) {
      if ((col+1) == ignore) continue;
      first += col+1;
    }
  }
  return first;
}

void solve() {
  int result = 0;
  for (auto pattern: patterns) {
    int c = solve(pattern, 0);

    for (int row = 0; row < (int) pattern.size(); row++) {
      for (int col = 0; col < (int) pattern[0].size(); col++) {
        int current = 0;
        if (pattern[row][col] == '.') {
          pattern[row][col] = '#';
          current = solve(pattern, c);
          pattern[row][col] = '.';
        } else {
          pattern[row][col] = '.';
          current = solve(pattern, c);
          pattern[row][col] = '#';
        }
        if (current != 0) {
          if (current == c) current = 0;
        }

        if (current != 0) {
          result += current;
        }
      }
    }

  }
  cout << result / 2 << endl;
}

int main() {
  read();
  solve();
}

