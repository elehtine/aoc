#include <iostream>
#include <set>

#include "readwrite.h"

using namespace std;

struct Point {
  int row, col;

  Point north() const { return { row, col - 1 }; }
  Point east() const { return { row + 1, col }; }
  Point south() const { return { row, col + 1 }; }
  Point west() const { return { row - 1, col }; }
};

bool operator<(const Point& left, const Point& right) {
  if (left.row == right.row) return left.col < right.col;
  return left.row < right.row;
}
bool operator>(const Point& left, const Point& right) {
  return right < left;
}
bool operator<=(const Point& left, const Point& right) {
  return !(left > right);
}
bool operator>=(const Point& left, const Point& right) {
  return right <= left;
}

vector<string> garden; 
set<Point> reach;

void read() {
  garden = read_lines();
}

Point start_position() {
  for (int row = 0; row < (int) garden.size(); row++) {
    for (int col = 0; col < (int) garden[0].size(); col++) {
      Point point = { row, col };
      if (garden[row][col] == 'S') {
        reach.insert(point);
        return point;
      }
    }
  }
  
  return { -1, -1 };
}

bool check(const Point& point) {
  if (point.row < 0 || point.col < 0) return false;
  if (
    point.row >= (int) garden.size() ||
    point.col >= (int) garden[0].size()
  ) {
    return false;
  }
  return garden[point.row][point.col] != '#';
}

set<Point> step() {
  set<Point> current;
  for (const Point& point: reach) {
    if (check(point.north())) current.insert(point.north());
    if (check(point.east())) current.insert(point.east());
    if (check(point.south())) current.insert(point.south());
    if (check(point.west())) current.insert(point.west());
  }
  return current;
}

long count(const Point& point, const int steps) {
  return 0L;
}

void first() {
  start_position();
  const int count = 64;
  for (int i = 0; i < count; i++) {
    reach = step();
  }
  cout << reach.size() << endl;
}

void second() {
  const int steps = 16;
  cout << count(start_position(), steps) << endl;
}

int main() {
  read();
  second();
}
