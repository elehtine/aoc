#include <iostream>
#include <algorithm>

#include "readwrite.h"

using namespace std;

enum class Direction {
  north, east, south, west,
};

Direction first_direction(string s) {
  if (s == "U") return Direction::north;
  if (s == "R") return Direction::east;
  if (s == "D") return Direction::south;
  if (s == "L") return Direction::west;
  cout << "INVALID DIRECTION" << endl;
  return Direction::north;
}

Direction second_direction(string s) {
  if (s[7] == '3') return Direction::north;
  if (s[7] == '0') return Direction::east;
  if (s[7] == '1') return Direction::south;
  if (s[7] == '2') return Direction::west;
  cout << "INVALID DIRECTION" << endl;
  return Direction::north;
}

ostream& operator<<(ostream& out, const Direction& dir) {
  if (dir == Direction::north) out << "NORTH";
  if (dir == Direction::east) out << "EAST ";
  if (dir == Direction::south) out << "SOUTH";
  if (dir == Direction::west) out << "WEST ";
  return out;
};

struct Step {
  Direction dir;
  int length;
};

Step first_step(string s) {
  vector<string> parts = split(s, " ");
  return { first_direction(parts[0]), stoi(parts[1]) };
}

int from_hex(string s) {
  int result = 0;
  for (int index = 2; index < (int) s.size() - 2; index++) {
    result *= 16;
    if ('0' <= s[index] && s[index] <= '9') result += s[index] - '0';
    if ('a' <= s[index] && s[index] <= 'f') result += s[index] - 'a' + 10;
  }
  return result;
}

Step second_step(string s) {
  vector<string> parts = split(s, " ");
  return { second_direction(parts[2]), from_hex(parts[2]) };
}

ostream& operator<<(ostream& out, const Step& step) {
  out << step.dir << " " << step.length;
  return out;
};

struct Point {
  long x, y;
};

Point point(Step step) {
  if (step.dir == Direction::north) return { 0, - step.length };
  if (step.dir == Direction::east) return { step.length, 0 };
  if (step.dir == Direction::south) return { 0, step.length };
  if (step.dir == Direction::west) return { - step.length, 0 };
  cout << "INVALID DIRECTION" << endl;
  return { -1, -1 };
}

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.x << "," << point.y;
  out << ")";
  return out;
};

Point& operator+=(Point& left, const Point& right) {
  left.x += right.x;
  left.y += right.y;
  return left;
}
Point operator+(Point left, const Point& right) {
  left += right;
  return left;
}
Point& operator-=(Point& left, const Point& right) {
  return left += { -right.x, -right.y };
}
Point operator-(Point left, const Point& right) {
  left -= right;
  return left;
}

vector<Point> points;
vector<Step> steps;

void read(bool first) {
  vector<string> lines = read_lines();
  for (const string& line: lines) {
    if (first) steps.push_back(first_step(line));
    else steps.push_back(second_step(line));
  }
}

long signed_area(const Point first, const Point second) {
  return (first.y * second.x) - (first.x * second.y);
}

long area() {
  long result = 0;
  for (int index = 1; index < (int) points.size(); index++) {
    result += signed_area(points[index], points[index-1]);
  }
  return result / 2;
}

long solve() {
  Point last { 0, 0 };
  for (const Step& step: steps) {
    points.push_back(last);
    last += point(step);
  }


  long result = 1;
  long perimeter = 0;
  for (int index = 0; index < (int) points.size(); index++) {
    perimeter += steps[index].length;
  }
  result += perimeter / 2;
  result += area();
  return result;
}

int main() {
  read(false);
  cout << solve() << endl;
}
