#include <iostream>
#include <array>
#include <queue>
#include <unordered_set>
#include <set>

#include "readwrite.h"

using namespace std;

const int NONE = -1;
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

struct Move {
  int direction;
};

ostream& operator<<(ostream& out, const Move& move) {
  if (move.direction == NONE) out << "#";
  if (move.direction == NORTH) out << "N";
  if (move.direction == EAST) out << "E";
  if (move.direction == SOUTH) out << "S";
  if (move.direction == WEST) out << "W";
  return out;
}

struct Path {
  vector<Move> moves;
  int x, y;
  int sum;
};

ostream& operator<<(ostream& out, const Path& path) {
  /*
  out << path.moves;
  out << "(";
  out << path.x << "," << path.y;
  out << "): ";
  */
  out << path.sum;
  return out;
}

bool operator<(const Path& left, const Path& right) {
  return left.sum - left.x - left.y > right.sum - right.x - right.y;
}

vector<string> blocks;
set<array<int, 4>> visited;
priority_queue<Path> paths;
int width, height;

void read() {
  blocks = read_lines();
  width = blocks[0].size();
  height = blocks.size();
}

int last(Path path) {
  return path.moves[path.moves.size() - 1].direction;
}

int get(int x, int y) {
  if (x < 0 || y < 0) return 0;
  if (x >= width || y >= height) return 0;
  return blocks[y][x] - '0';
}

int streak(Path path) {
  int n = path.moves.size();
  for (int i = 1; i < n; i++) {
    if (path.moves[n-i].direction != path.moves[n-i-1].direction) {
      return i;
    }
  }
  return n;
}

Path move(Path last, int direction) {
  if (direction == NORTH) last.y--;
  if (direction == EAST) last.x++;
  if (direction == SOUTH) last.y++;
  if (direction == WEST) last.x--;

  last.moves.push_back({ direction });
  last.sum += get(last.x, last.y);
  return last;
}

void append(Path path, int direction, int low, int high) {
  bool same = last(path) == NONE || last(path) == direction;

  if (streak(path) < low) {
    if (same) {
      paths.push(move(path, direction));
    }
  }
  if (low <= streak(path) && streak(path) < high) {
    paths.push(move(path, direction));
  }
  if (streak(path) == high && !same) {
    paths.push(move(path, direction));
  }
}

void search(Path current, int low, int high) {
  int l = last(current);
  if (l == NONE) {
    append(current, EAST, low, high);
    append(current, SOUTH, low, high);
  }

  if (l == NORTH) {
    append(current, NORTH, low, high);
    append(current, EAST, low, high);
    append(current, WEST, low, high);
  }
  if (l == SOUTH) {
    append(current, SOUTH, low, high);
    append(current, EAST, low, high);
    append(current, WEST, low, high);
  }
  if (l == EAST) {
    append(current, EAST, low, high);
    append(current, NORTH, low, high);
    append(current, SOUTH, low, high);
  }
  if (l == WEST) {
    append(current, WEST, low, high);
    append(current, NORTH, low, high);
    append(current, SOUTH, low, high);
  }
}

void solve(int low, int high) {
  paths.push({ { { NONE } }, 0, 0, 0 });
  while (!paths.empty()) {
    Path current = paths.top();
    paths.pop();

    if (current.x == width-1 && current.y == height-1) {
      if (streak(current) < low) continue;
      cout << current << endl;
      break;
    }
    if (current.x < 0 || current.y < 0) continue;
    if (current.x >= width || current.y >= height) continue;

    array<int, 4> index = {
      current.x, current.y, last(current), streak(current)
    };
    if (visited.count(index)) continue;
    visited.insert(index);

    search(current, low, high);
  }
}

void first() {
  solve(1, 3);
}

void second() {
  solve(4, 10);
}

int main() {
  read();
  second();
}

