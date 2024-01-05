#include <iostream>

#include "readwrite.h"

using namespace std;

vector<string> trails;
vector<vector<bool>> visited;
int width, height;

void read() {
  trails = read_lines();
  width = trails[0].size();
  height = trails.size();
  visited = vector<vector<bool>>(height, vector<bool>(width, false));
}

int depth_first(int row, int col, int steps, bool second) {
  if (row == height-1 && col == width-2) {
    return steps;
  }
  if (row < 0 || row >= width) return -1;
  if (col < 0 || col >= height) return -1;

  char current = trails[row][col];
  if (current == '#') return -1;

  if (visited[row][col]) return -1;
  visited[row][col] = true;

  int result = -1;
  if (second || current == '.' || current == '>') {
    result = max(result, depth_first(row, col+1, steps+1, second));
  }
  if (second || current == '.' || current == 'v') {
    result = max(result, depth_first(row+1, col, steps+1, second));
  }
  if (second || current == '.') {
    result = max(result, depth_first(row-1, col, steps+1, second));
    result = max(result, depth_first(row, col-1, steps+1, second));
  }

  visited[row][col] = false;
  return result;
}

void first() {
  cout << "first: " << depth_first(0, 1, 0, false) << endl;
}

void second() {
  cout << "second: " << depth_first(0, 1, 0, true) << endl;
}

int main() {
  read();
  first();
  second();
}
