#include <iostream>

#include "readwrite.h"

using namespace std;

struct Point {
  int row, col;

  Point east() const;
  Point south() const;
  Point west() const;
  Point north() const;
};

Point Point::east() const { return { row, col + 1 }; }
Point Point::south() const { return { row + 1, col }; }
Point Point::west() const { return { row, col -  1 }; }
Point Point::north() const { return { row - 1, col }; }

bool operator==(const Point& left, const Point& right) {
  return left.row == right.row && left.col == right.col;
}

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.row << "," << point.col;
  out << ")";
  return out;
}

vector<Point> nodes;
vector<vector<int>> edges;
vector<bool> visited_nodes;

vector<string> trails;
vector<vector<bool>> visited;
int width, height;

void read() {
  trails = read_lines();
  width = trails[0].size();
  height = trails.size();
  visited = vector<vector<bool>>(height, vector<bool>(width, false));

  nodes.push_back({ 0, 1 });
  nodes.push_back({ height-1, width-2 });
  for (int row = 1; row < height-1; row++) {
    for (int col = 1; col < width-1; col++) {
      if (trails[row][col] != '.') continue;
      int count = 0;
      if (trails[row+1][col] == '#') count++;
      if (trails[row-1][col] == '#') count++;
      if (trails[row][col+1] == '#') count++;
      if (trails[row][col-1] == '#') count++;
      if (count < 2) nodes.push_back({ row, col });
    }
  }
  visited_nodes = vector<bool>(nodes.size(), false);
  edges = vector<vector<int>>(nodes.size(), vector<int>(nodes.size(), -1));
}

pair<int, int> depth_first(Point here, int steps, bool second) {
  int row = here.row;
  int col = here.col;

  if (row < 0 || row >= width) return { -1, -1 };
  if (col < 0 || col >= height) return { -1, -1 };

  char current = trails[row][col];
  if (current == '#') return { -1, -1 };

  if (visited[row][col]) return { -1, -1 };
  for (int node = 0; node < (int) nodes.size(); node++) {
    if (here == nodes[node]) return { node, steps };
  }
  visited[row][col] = true;

  pair<int, int> result { -1, -1 };
  if (second || current == '.' || current == '>') {
    auto east = depth_first(here.east(), steps+1, second);
    if (east.first != -1) result = east;
  }
  if (second || current == '.' || current == 'v') {
    auto south = depth_first(here.south(), steps+1, second);
    if (south.first != -1) result = south;
  }
  if (second || current == '.') {
    auto west = depth_first(here.west(), steps+1, second);
    auto north = depth_first(here.north(), steps+1, second);
    if (west.first != -1) result = west;
    if (north.first != -1) result = north;
  }

  visited[row][col] = false;
  return result;
}

int depth_first(int from, int steps) {
  if (from == 1) {
    return steps;
  }
  if (visited_nodes[from]) return -1;
  visited_nodes[from] = true;

  int result = -1;
  for (int to = 0; to < (int) nodes.size(); to++) {
    int distance = edges[from][to];
    if (distance == -1) continue;
    result = max(result, depth_first(to, steps + distance));
  }

  visited_nodes[from] = false;
  return result;
}


void add(int from, int to, int distance) {
  edges[from][to] = distance;
}

void construct(bool second) {
  for (int from = 0; from < (int) nodes.size(); from++) {
    visited[nodes[from].row][nodes[from].col] = true;
    pair<int, int> east = depth_first(nodes[from].east(), 1, second);
    pair<int, int> south = depth_first(nodes[from].south(), 1, second);
    pair<int, int> west = depth_first(nodes[from].west(), 1, second);
    pair<int, int> north = depth_first(nodes[from].north(), 1, second);
    visited[nodes[from].row][nodes[from].col] = false;
    if (east.first != -1) add(from, east.first, east.second);
    if (south.first != -1) add(from, south.first, south.second);
    if (west.first != -1) add(from, west.first, west.second);
    if (north.first != -1) add(from, north.first, north.second);
  }
}

void first() {
  construct(false);
  cout << depth_first(0, 0);
}

void second() {
  construct(true);
  cout << depth_first(0, 0);
}

int main() {
  read();
  second();
}
