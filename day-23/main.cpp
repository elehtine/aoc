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

int depth_first(int row, int col, int steps) {
  if (col == height-1) return steps;
  if (row < 0 || row >= width) return -1;
  if (col < 0 || col >= height) return -1;
  if (visited[row][col]) return -1;
  visited[row][col] = true;

  char current = trails[row][col];
  if (current == '#') return -1;

  int result = -1;
  if (current == '.' || current == '>') result = max(result, depth_first(row, col+1, steps+1));
  if (current == '.' || current == 'v') result = max(result, depth_first(row+1, col, steps+1));
  if (current == '.') {
    result = max(result, depth_first(row-1, col, steps+1));
    result = max(result, depth_first(row, col-1, steps+1));
  }

  visited[row][col] = false;
  return result;
}

void first() {
  cout << depth_first(1, 1, 0) << endl;
}

int main() {
  read();
  first();
}
