#include <iostream>

#include "readwrite.h"

using namespace std;

struct Point {
  long x, y, z;

  Point() = default;
  Point(long x, long y, long z);
  Point(string cors);
};

Point::Point(long x, long y, long z): x(x), y(y), z(z) {};

Point::Point(string cors) {
  vector<string> parts = split(cors, ", ");
  x = stol(parts[0]);
  y = stol(parts[1]);
  z = stol(parts[2]);
}

bool operator<(const Point& left, const Point& right) { return left.x < right.x; }

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.x << "," << point.y << "," << point.z;
  out << ")";
  return out;
}

struct Line {
  Point start, velocity;

  Line(string line);
  bool intersect(const Line& other) const;
};

bool operator<(const Line& left, const Line& right) { return left.start < right.start; }

ostream& operator<<(ostream& out, const Line& line) {
  out << line.start << " @ " << line.velocity;
  return out;
}

Line::Line(string line) {
  vector<string> parts = split(line, " @ ");
  start = Point { parts[0] };
  velocity = Point { parts[1] };
}

// const long low = 7;
// const long high = 27;
const long low = 200'000'000'000'000;
const long high = 400'000'000'000'000;

/**
 * x1 + n*vx1 = x2 + m*vx2
 * y1 + n*vy1 = y2 + m*vy2
 * n = (dx + m*vx2) / vx1 = (dy + m*vy2) / vy1
 * (dx + m*vx2) * vy1 = (dy + m*vy2) * vx1
 * m * (vx2*vy1 - vy2*vx1) = dy*vx1 - dx*vy1
 * m = dy*vx1 - dx*vy1 / (vx2*vy1 - vy2*vx1)
 */
bool Line::intersect(const Line& other) const {
  long dy = other.start.y - start.y;
  long dx = other.start.x - start.x;
  long denominator = other.velocity.y*velocity.x - other.velocity.x*velocity.y ;
  if (denominator == 0) {
    cout << *this << endl;
    cout << other << endl;
    cout << endl;
    return false;
  }
  long sign = denominator > 0 ? 1 : -1;

  long m = (dy*velocity.x - dx*velocity.y);
  long n = (dx*other.velocity.y - dy*other.velocity.x);
  if (m * sign * -1 < 0 || n * sign < 0) return false;

  double x = ((double) n*velocity.x / denominator + start.x);
  double y = ((double) n*velocity.y / denominator + start.y);
  if (x < low || x > high) return false;
  if (y < low || y > high) return false;
  return true;
}

vector<Line> hails;

void read() {
  vector<string> lines = read_lines();
  for (const string& line: lines) {
    hails.push_back(Line { line });
  }
}

void first() {
  int counter = 0;
  for (int first = 0; first < (int) hails.size(); first++) {
    for (int second = first + 1; second < (int) hails.size(); second++) {
      Line left = hails[first];
      Line right = hails[second];
      if (right < left) swap(left, right);
      if (left.intersect(right)) counter++;
    }
  }
  cout << counter << endl;
}

bool intersect(const Line& left, const Line& right) {
  if (left.velocity.x == right.velocity.x) return false;
  if (left.velocity.y == right.velocity.y) return false;
  if (left.velocity.z == right.velocity.z) return false;

  long dx = right.start.x - left.start.x;
  long dy = right.start.y - left.start.y;
  long dz = right.start.z - left.start.z;
  long dvx = left.velocity.x - right.velocity.x;
  long dvy = left.velocity.y - right.velocity.y;
  long dvz = left.velocity.z - right.velocity.z;
  if (dvx * dx < 0 || dvy * dy < 0 || dvz * dz < 0) return false;

  long nx = (long) dx / dvx;
  long ny = (long) dy / dvy;
  long nz = (long) dz / dvz;
  if (abs(nx - ny) < 100 && abs(nx - nz) < 100 && abs(ny - nz) < 100) {
    cout << nx << " " << ny << " " << nz << endl;
    cout << nx-ny << " " << nx-nz << " " << ny-nz << endl;
    cout << endl;
    return true;
  }
  return false;
}

double distance(const Line& left, const Line& right, Point projection) {
  long dx = right.start.x - left.start.x;
  long dy = right.start.y - left.start.y;
  long dz = right.start.z - left.start.z;
  projection.x = projection.x * projection.x;
  projection.y = projection.y * projection.y;
  projection.z = projection.z * projection.z;
  dx = dx * dx;
  dy = dy * dy;
  dz = dz * dz;
  double d = projection.x + projection.y + projection.z;
  return (projection.x * dx + projection.y * dy + projection.z * dz) / d;
}

void second() {
  long counter = 0;
  const long limit = 200;
  for (long x = -limit; x <= limit; x++) {
    for (long y = -limit; y <= limit; y++) {
      for (long z = -limit; z <= limit; z++) {
        if (x == 0 && y == 0 && z == 0) continue;
        double best = 0;
        double threshold = 1;
        bool running = true;
        for (int first = 0; running && first < (int) hails.size(); first++) {
          for (int second = first + 1; running && second < (int) hails.size(); second++) {
            Line left = hails[first];
            Line right = hails[second];
            double d = distance(left, right, { x, y, z });
            best = max(best, d);
            if (d > threshold) running = false;
          }
        }
        if (best > threshold) continue;
        cout << Point { x, y, z } << endl;
        cout << best << endl;
      }
    }
  }
  // cout << counter << " / " << hails.size() * (hails.size() - 1) / 2 << endl;
}

int main() {
  read();
  second();
}
