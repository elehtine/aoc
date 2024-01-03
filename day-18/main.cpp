#include <iostream>
#include <algorithm>

#include "readwrite.h"

using namespace std;

struct Point {
  long x, y;
};

bool operator==(const Point& left, const Point& right) {
  return left.x == right.x && left.y == right.y;
}

Point& operator+=(Point& left, const Point& right) {
  left = { left.x + right.x, left.y + right.y };
  return left;
}

Point operator+(const Point& left, const Point& right) {
  return { left.x + right.x, left.y + right.y };
}

bool operator<(const Point& left, const Point& right) {
  if (left.y == right.y) return left.x < right.x;
  return left.y < right.y;
}

bool operator>(const Point& left, const Point& right) {
  return right < left;
}

struct Line {
  Point start, end;

  long width() { return abs(start.x - end.x); };
};

struct Instruction {
  string direction;
  long length;
  Line line;

  long height() const {
    return max(line.start.y, line.end.y);
  }
  long lower() const {
    return min(line.start.y, line.end.y);
  }
};

bool operator<(const Instruction& left, const Instruction& right) {
  if (left.height() == right.height()) {
    if (left.lower() != right.lower()) {
      return left.lower() < right.lower();
    }
    if (left.direction == "R" || left.direction == "L") return true;
    if (right.direction == "R" || right.direction == "L") return false;
    return left.line.start < right.line.start;
  }
  return left.height() < right.height();
}

ostream& operator<<(ostream& out, const Point& point) {
  out << "(";
  out << point.x << "," << point.y;
  out << ")";
  return out;
}

ostream& operator<<(ostream& out, const Line& line) {
  out << line.start << " - " << line.end;
  return out;
}

ostream& operator<<(ostream& out, const Instruction& instruction) {
  out << instruction.direction << " ";
  out << instruction.length << " ";
  out << instruction.height() << " ";
  out << instruction.line;
  return out;
}

vector<Instruction> plan;
vector<Line> status;

long to_int(string hex) {
  long num = 0;
  for (int i = 0; i < 5; i++) {
    num *= 16;
    if ('0' <= hex[i] && hex[i] <= '9') num += hex[i] - '0';
    if ('a' <= hex[i] && hex[i] <= 'f') num += hex[i] - 'a' + 10;
  }
  return num;
}

Point move(Instruction instruction, Point point) {
  if (instruction.direction == "R") point += { instruction.length, 0 };
  if (instruction.direction == "D") point += { 0, instruction.length };
  if (instruction.direction == "L") point += { -instruction.length, 0 };
  if (instruction.direction == "U") point += { 0, -instruction.length };
  return point;
}

Instruction first_instruction(string line, Point from) {
  vector<string> parts = split(line, " ");
  Instruction instruction = {
    parts[0], stoi(parts[1]), { from, from }
  };
  instruction.line.end = move(instruction, from);
  if (instruction.line.start > instruction.line.end) {
    swap(instruction.line.start, instruction.line.end);
  }
  return instruction;
}

Instruction second_instruction(string line, Point from) {
  vector<string> parts = split(line, " ");
  string direction;
  if (parts[2][7] == '0') direction = "R";
  if (parts[2][7] == '1') direction = "D";
  if (parts[2][7] == '2') direction = "L";
  if (parts[2][7] == '3') direction = "U";

  string hex = parts[2].substr(2, 5);
  Instruction instruction = {
    direction, to_int(hex), { from, from }
  };
  instruction.line.end = move(instruction, from);
  if (instruction.line.start > instruction.line.end) {
    swap(instruction.line.start, instruction.line.end);
  }
  return instruction;
}

void read() {
  vector<string> lines = read_lines();
  Point current = { 0, 0 };
  for (int index = 0; index < (int) lines.size(); index++) {
    string line = lines[index];
    Instruction instruction = first_instruction(line, current);
    current = move(instruction, current);
    plan.push_back(instruction);
  }
}

int check_index(Instruction instruction) {
  for (int i = 0; i < (int) status.size(); i++) {
    if (instruction.line.start.x == status[i].start.x) return i;
    if (instruction.line.start.x == status[i].end.x) return i;
    if (instruction.line.end.x == status[i].end.x) return i;
    if (instruction.line.end.x == status[i].start.x) return i;
  }
  return -1;
}

long add(Instruction instruction) {
  for (int i = 0; i < (int) status.size(); i++) {
    Line line = status[i];
    bool start = line.start.x < instruction.line.start.x;
    bool end = instruction.line.end.x < line.end.x ;
    bool between = start && end;
    if (between) {
      int height = abs(line.start.y - instruction.line.start.y);

      Line first = line;
      Line second = line;
      first.end.x = instruction.line.start.x;
      second.start.x = instruction.line.end.x;

      status.erase(status.begin() + i);
      status.insert(status.begin() + i, first);
      status.insert(status.begin() + i + 1, second);
      return (instruction.line.width() - 1) * height;
    }
  }

  for (int i = 0; i < (int) status.size(); i++) {
    if (instruction.line.end.x < status[i].start.x ) {
      if (i > 0 && instruction.line.start.x > status[i-1].end.x) {
        cout << "------- WRONG HERE -------" << endl;
      }
      status.insert(status.begin() + i, instruction.line);
      return instruction.line.width() + 1;
    }
  }

  status.push_back(instruction.line);
  return instruction.line.width() + 1;
}

void remove(int index) {
  status.erase(status.begin() + index);
}

void solve() {
  sort(plan.begin(), plan.end());
  status.push_back( plan[0].line );
  plan.erase(plan.begin());
  long result = status[0].width() + 1;

  for (Instruction& instruction: plan) {
    bool down = instruction.direction == "D";
    bool right = instruction.direction == "R";
    bool up = instruction.direction == "U";
    bool left = instruction.direction == "L";

    int index = check_index(instruction);

    cout << instruction << endl;
    cout << "status:" << endl;
    cout << status << endl;

    if (index == -1) {
      result += add(instruction);
    }


    long width = 0;
    if (index != -1) {
      Line& line = status[index];
      long height = instruction.line.end.y - line.start.y;
      width = line.width() + 1;

      if (down || up) {
        long area = abs(width * height);
        result += area;
        line.start.y = instruction.line.end.y;
        line.end.y = instruction.line.end.y;
      }

      if (left || right) { // left
        if (instruction.line.start == line.start) {
          line.start.x += instruction.length;
        } else if (instruction.line.start == line.end) {
          line.end.x += instruction.length;
          result += instruction.length;
        } else if (instruction.line.end == line.start) {
          line.start.x -= instruction.length;
          result += instruction.length;
        } else if (instruction.line.end == line.end) {
          line.end.x -= instruction.length;
        }
      }

      if (
        index < (int) status.size() -1 &&
        status[index].end == status[index+1].start
      ) {
        status[index].end = status[index+1].start;
        remove(index+1);
        result--;
      }
      if (index > 0 && status[index-1].end == status[index].start) {
        status[index-1].end = status[index].end;
        remove(index-1);
        result--;
      }

      if (line.width() == 0) {
        remove(index);
        if (index < (int) status.size() && status[index].width() == 0) {
          remove(index);
        }
        if (index > 0 && status[index-1].width() == 0) {
          remove(index-1);
        }
      }
    }
  }
  cout << "result: " << result << endl;
}

int main() {
  read();
  solve();
}
