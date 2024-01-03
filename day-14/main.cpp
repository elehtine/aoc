#include <iostream>

#include "readwrite.h"

using namespace std;

vector<string> rocks;
int n;
int m;

const int N = 1'000'000'000;
const int NORTH = 0;
const int WEST = 1;
const int SOUTH = 2;
const int EAST = 3;

void read() {
  rocks = read_lines();
  n = rocks.size();
  m = rocks[0].size();
}

void move(vector<string>& v, int direction) {
  if (direction == NORTH || direction == WEST) {
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < m; col++) {
        if (v[row][col] != 'O') continue;

        for (int d = 1; ; d++) {
          int next_row = row, next_col = col;
          int last_row = row, last_col = col;
          if (direction == NORTH) {
            next_row = row-d;
            last_row = row-d+1;
          }
          if (direction == WEST) {
            next_col = col-d;
            last_col = col-d+1;
          }

          if (next_row < 0 || next_col < 0) break;
          if (v[next_row][next_col] != '.') break;

          v[next_row][next_col] = 'O';
          v[last_row][last_col] = '.';
        }
      }
    }
    return;
  }

  for (int row = n-1; row >= 0; row--) {
    for (int col = m-1; col >= 0; col--) {
      if (v[row][col] != 'O') continue;

      for (int d = 1; ; d++) {
        int next_row = row, next_col = col;
        int last_row = row, last_col = col;
        if (direction == SOUTH) {
          next_row = row+d;
          last_row = row+d-1;
        }
        if (direction == EAST) {
          next_col = col+d;
          last_col = col+d-1;
        }

        if (next_row >= n || next_col >= m) break;
        if (v[next_row][next_col] != '.') break;

        v[next_row][next_col] = 'O';
        v[last_row][last_col] = '.';
      }
    }
  }
}

long result() {
  long result = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < m; col++) {
      if (rocks[row][col] == 'O') result += n-row;
    }
  }
  return result;
}

int main() {
  read();
  vector<string> fast(rocks);
  vector<string> slow(rocks);
  int round = 0;
  int start = 0;
  for (int counter = 1; counter < N; counter++) {
    move(fast, NORTH);
    move(fast, WEST);
    move(fast, SOUTH);
    move(fast, EAST);

    move(fast, NORTH);
    move(fast, WEST);
    move(fast, SOUTH);
    move(fast, EAST);

    move(slow, NORTH);
    move(slow, WEST);
    move(slow, SOUTH);
    move(slow, EAST);

    if (fast == slow) {
      if (start == 0) {
        start = counter;
        continue;
      }

      round = (N-start) / (counter-start) * (counter-start);
      break;
    }
  }

  for (; round < N; round++) {
    move(rocks, NORTH);
    move(rocks, WEST);
    move(rocks, SOUTH);
    move(rocks, EAST);
  }
  cout << result() << endl;
}

