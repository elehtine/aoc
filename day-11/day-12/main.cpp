#include <iostream>
#include <vector>
#include <string>

#include "readwrite.h"

#define K 1'000'000

using namespace std;

struct Point {
  long row, col;
};

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.row << "," << point.col;
  out << ")";
  return out;
}

int rows, cols;
vector<string> space;
vector<Point> galaxies;
vector<long> row_table;
vector<long> col_table;

void read() {
  space = read_lines();
  rows = space.size();
  cols = space[0].size();

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      if (space[row][col] == '#') galaxies.push_back({ row, col });
    }
  }

  row_table.resize(rows+1);
  col_table.resize(cols+1);

  for (int row = 0; row < rows; row++) {
    bool no = true;
    for (int col = 0; col < cols; col++) {
      if (space[row][col] == '#') no = false;
    }
    row_table[row+1] = row_table[row];
    if (no) row_table[row+1]++;
  }

  for (int col = 0; col < cols; col++) {
    bool no = true;
    for (int row = 0; row < rows; row++) {
      if (space[row][col] == '#') no = false;
    }
    col_table[col+1] = col_table[col];
    if (no) col_table[col+1]++;
  }

}

long distance(const Point& first, const Point& second) {
  long result = abs(first.row - second.row) + abs(first.col - second.col);
  result += abs(row_table[first.row] - row_table[second.row]) * (K - 1);
  result += abs(col_table[first.col] - col_table[second.col]) * (K - 1);
  return result;
}

void solve() {
  long result = 0;
  for (int first = 0; first < (int) galaxies.size(); first++) {
    for (int second = first+1; second < (int) galaxies.size(); second++) {
      result += distance(galaxies[first], galaxies[second]);
    }
  }
  cout << result << endl;
}

int main() {
  read();
  solve();
}

