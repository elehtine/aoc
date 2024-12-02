#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Point {
  int x, y;
  Point right();
  Point left();
  Point up();
  Point down();

  bool friend operator<(const Point& left, const Point& right);
  bool friend operator>(const Point& left, const Point& right);
  bool friend operator<=(const Point& left, const Point& right);
  bool friend operator>=(const Point& left, const Point& right);
};

Point Point::right() { return Point { x+1, y }; }
Point Point::left() { return Point { x-1, y }; }
Point Point::up() { return Point { x, y-1 }; }
Point Point::down() { return Point { x, y+1 }; }

bool operator<(const Point& left, const Point& right) {
  if (left.x == right.x) return left.y < right.y;
  return left.x < right.x;
}
bool operator>(const Point& left, const Point& right) { return right < left; }
bool operator<=(const Point& left, const Point& right) { return !(left > right); }
bool operator>=(const Point& left, const Point& right) { return !(left < right); }

vector<string> grid;
map<Point, int> dist;

void print() {
  for (const string& line: grid) {
    cout << line << endl;
  }
}

void print_dist() {
  for (int y = 0; y < (int) grid.size(); y++) {
    for (int x = 0; x < (int) grid[0].size(); x++) {
      Point p { x, y };
      cout << dist[p];
    }
    cout << endl;
  }
}

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.x << "," << point.y;
  out << ")";
  return out;
}

Point find_start() {
  for (int y = 0; y < (int) grid.size(); y++) {
    for (int x = 0; x < (int) grid[0].size(); x++) {
      if (grid[y][x] == 'S') return Point { x, y };
    }
  }
  return Point { -1, -1 };
}

void read() {
  string line;
  while (getline(cin, line)) {
    grid.push_back(line);
  }
}

bool check(Point from, Point to, int d) {
  if (dist[to] != 0 && dist[to] < d) return false;
  if (to.x < 0 || to.x >= (int) grid[0].size()) return false;
  if (to.y < 0 || to.y >= (int) grid.size()) return false;

  string right = "FL-";
  string left = "7J-";
  string up = "LJ|";
  string down = "F7|";

  string from_check = "";
  string to_check = "";
  if (to.x - from.x == -1) {
    from_check = left;
    to_check = right;
  }
  if (to.x - from.x == 1) {
    from_check = right;
    to_check = left;
  }
  if (to.y - from.y == -1) {
    from_check = up;
    to_check = down;
  }
  if (to.y - from.y == 1) {
    from_check = down;
    to_check = up;
  }

  char c_to = grid[to.y][to.x];
  char c_from = grid[from.y][from.x];

  bool reject = true;
  if (c_from == 'S') reject =  false;
  for (const char& c: from_check) {
    if (c_from == c) reject = false;
  }
  if (reject) return false;

  reject = true;
  for (const char& c: to_check) {
    if (c_to == c) reject = false;
  }
  if (reject) return false;

  return true;
}

void df(Point point, int d) {
  dist[point] = d;
  if (check(point, point.right(), d)) df(point.right(), d+1);
  if (check(point, point.left(), d)) df(point.left(), d+1);
  if (check(point, point.up(), d)) df(point.up(), d+1);
  if (check(point, point.down(), d)) df(point.down(), d+1);
}

void first() {
  Point start = find_start();
  cout << "start: " << start << endl;
  df(start, 1);
  int result = 0;
  for (int y = 0; y < (int) grid.size(); y++) {
    for (int x = 0; x < (int) grid[0].size(); x++) {
      Point p { x, y };
      result = max(result, dist[p]);
    }
  }
  cout << "result: " << result-1 << endl;
}

void second() {
  int result = 0;
  int pipes = 0;
  for (int y = 0; y < (int) grid.size(); y++) {
    for (int x = 0; x < (int) grid[0].size(); x++) {
      Point p { x, y };
      if (dist[p]) {
        const char& c = grid[y][x];
        string wall = "|SF7";
        for (const char& w: wall) {
          if (c == w) pipes++;
        }
      } else {
        if (pipes % 2 == 1) {
          result++;
        } else {
        }
      }
    }
  }
  cout << "result: " << result << endl;
}

int main() {
  read();
  // print();
  first();
  // print_dist();
  second();
}

