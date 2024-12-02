#include <iostream>
#include <algorithm>
#include <set>

#include "readwrite.h"

using namespace std;

struct Point {
  int x, y, z;

  Point() = default;
  Point(string s);
  int distance(Point p);
};

Point::Point(string s) {
  vector<string> parts = split(s, ",");
  x = stoi(parts[0]);
  y = stoi(parts[1]);
  z = stoi(parts[2]);
}

int Point::distance(Point p) {
  return (abs(x - p.x) + 1) * (abs(y - p.y) + 1) * (abs(z - p.z) + 1);
}

bool operator==(const Point& left, const Point& right) {
  if (left.x != right.x) return false;
  if (left.y != right.y) return false;
  if (left.z != right.z) return false;
  return true;
}
bool operator!=(const Point& left, const Point& right) {
  return !(left == right);
}

bool operator<(const Point& left, const Point& right) {
  return left.z < right.z;
}

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.x << "," << point.y << "," << point.z;
  out << ")";
  return out;
}

enum class Dir {
  x_horisontal, y_horisontal, vertical
};

ostream& operator<<(ostream& out, const Dir& dir) {
  if (dir == Dir::x_horisontal) out << "x-horisontal";
  if (dir == Dir::y_horisontal) out << "y-horisontal";
  if (dir == Dir::vertical) out << "vertical";
  return out;
}

Dir direction(const Point& left, const Point& right) {
  if (left.x == right.x && left.y == right.y) return Dir::vertical;
  if (left.x == right.x && left.z == right.z) return Dir::y_horisontal;
  if (left.y == right.y && left.z == right.z) return Dir::x_horisontal;
  return Dir::x_horisontal;
}

struct Brick {
  Point start;
  Dir dir;
  int length;
  bool important;

  Brick(string line);
  int height();
  bool support(const Brick& brick) const;
};

Brick::Brick(string line) {
  vector<string> parts = split(line, "~");
  start = { parts[0] };
  Point end { parts[1] };
  if (end < start) swap(start, end);
  length = start.distance(end);
  dir = direction(start, end);
}

int Brick::height() {
  int add = dir == Dir::vertical ? length - 1 : 0;
  return start.z + add;
}

bool Brick::support(const Brick& brick) const {
  vector<Point> base;
  vector<Point> other;
  if (dir == Dir::vertical) {
    Point point = start;
    point.z += length;
    base.push_back(point);
  } else {
    for (int d = 0; d < length; d++) {
      Point point = start;
      point.z++;
      if (dir == Dir::x_horisontal) point.x += d;
      if (dir == Dir::y_horisontal) point.y += d;
      base.push_back(point);
    }
  }

  if (brick.dir == Dir::vertical) other.push_back(brick.start);
  else {
    for (int d = 0; d < brick.length; d++) {
      Point point = brick.start;
      if (brick.dir == Dir::x_horisontal) point.x += d;
      if (brick.dir == Dir::y_horisontal) point.y += d;
      other.push_back(point);
    }
  }

  for (const Point& first: base) {
    for (const Point& second: other) {
      if (first == second) return true;
    }
  }
  return false;
}

bool operator==(const Brick& left, const Brick& right) {
  if (left.start != right.start) return false;
  if (left.dir != right.dir) return false;
  if (left.length != right.length) return false;
  return true;
}

bool operator<(const Brick& left, const Brick& right) {
  return left.start < right.start;
}

ostream& operator<<(ostream& out, const Brick& brick) {
  out << brick.start << " " << brick.dir << " " << brick.length;
  return out;
}

vector<Brick> bricks;
vector<vector<int>> heights;
set<int> must;
const int width = 10;

void read() {
  vector<string> lines = read_lines();
  for (string line: lines) {
    bricks.push_back(Brick(line));
  }
  heights = vector<vector<int>>(width, vector<int>(width, -1));
}

void ground() {
  for (int y = 0; y < width; y++) {
    for (int x = 0; x < width; x++) {
      if (heights[x][y] == -1) cout << 0;
      else cout << bricks[heights[x][y]].height();
    }
    cout << endl;
  }
  cout << endl;
}

int find_old(const Brick& brick, int d) {
  if (brick.dir == Dir::vertical) {
    return heights[brick.start.x][brick.start.y];
  }
  if (brick.dir == Dir::x_horisontal) {
    return heights[brick.start.x + d][brick.start.y];
  }
  if (brick.dir == Dir::y_horisontal) {
    return heights[brick.start.x][brick.start.y + d];
  }
  return -1;
}

void drop(int index) {
  Brick& brick = bricks[index];
  brick.start.z = 1;

  if (brick.dir == Dir::vertical) {
    int old = find_old(brick, 0);
    heights[brick.start.x][brick.start.y] = index;
    if (old == -1) return;


    must.insert(old);
    brick.start.z = bricks[old].height() + 1;
    return;
  }

  set<int> support;
  for (int d = 0; d < brick.length; d++) {
    int old = find_old(brick, d);
    if (old == -1) continue;
    if (brick.start.z < bricks[old].height() + 1) {
      brick.start.z = bricks[old].height() + 1;
      support.clear();
    }
    if (brick.start.z == bricks[old].height() + 1) support.insert(old);
  }
  if (support.size() == 1) {
    must.insert(*support.begin());
  }

  int x = brick.start.x;
  int y = brick.start.y;
  for (int d = 0; d < brick.length; d++) {
    if (brick.dir == Dir::x_horisontal) heights[x + d][y] = index;
    if (brick.dir == Dir::y_horisontal) heights[x][y + d] = index;
  }
}

void second() {
  int result = 0;
  for (int important: must) {
    Brick start = bricks[important];
    vector<Brick> stay;

    for (const Brick& brick: bricks) {
      if (brick == start) continue;
      if (brick.start.z == 1) stay.push_back(brick);
      for (int index = stay.size() - 1; index >= 0; index--) {
        Brick base = stay[index];
        if (base.support(brick)) {
          stay.push_back(brick);
          break;
        }
      }
    }

    result += bricks.size() - stay.size() - 1;
  }
  cout << "fall: " << result;
}

void first() {
  sort(bricks.begin(), bricks.end());
  for (int index = 0; index < (int) bricks.size(); index++) {
    drop(index);
  }
  cout << "safe: " << bricks.size() - must.size() << endl;
}

int main() {
  read();
  first();
  second();
}
