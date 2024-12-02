#include <iostream>
#include <map>

#include "readwrite.h"

using namespace std;

struct Lens {
  string label;
  int lens;
};

ostream& operator<<(ostream& out, const Lens& lens) {
  out << "(";
  out << lens.label << "," << lens.lens;
  out << ")";
  return out;
}

vector<string> sequence;
map<int, vector<Lens>> boxes;

void read() {
  string line;
  cin >> line;
  sequence = split(line, ",");
}

int hash_1A(string s) {
  int result = 0;
  for (const char& c: s) {
    result += c;
    result *= 17;
    result %= 256;
  }
  return result;
}

void first() {
  int result = 0;
  for (string line: sequence) {
    result += hash_1A(line);
  }
  cout << "result: " << result << endl;
}

int sign_index(string line) {
  for (int i = 0; i < (int) line.size(); i++) {
    if (line[i] == '-' || line[i] == '=') return i;
  }
  return -1;
}

void put(int box, const Lens& lens) {
  int n = boxes[box].size();
  for (int index = 0; index < n; index++) {
    if (boxes[box][index].label == lens.label) {
      boxes[box][index] = lens;
      return;
    }
  }
  boxes[box].push_back(lens);
}

void remove(int box, string label) {
  int n = boxes[box].size();
  for (int index = 0; index < n; index++) {
    if (boxes[box][index].label == label) {
      boxes[box].erase(boxes[box].begin() + index);
      break;
    }
  }
}

void second() {
  const char REMOVE = '-';
  const char PUT = '=';

  for (string line: sequence) {
    int sign = sign_index(line);
    string label = line.substr(0, sign);
    int box = hash_1A(label);
    char operation = line[sign];

    if (operation == PUT) {
      int number = stoi(line.substr(sign+1));
      const Lens lens { label, number };
      put(box, lens);
    }
    if (operation == REMOVE) {
      remove(box, label);
    }
  }

  int result = 0;
  for (int i = 0; i < 256; i++) {
    vector<Lens> box = boxes[i];
    if (!box.empty()) {
      int index = 1;
      for (Lens lens: box) {
        result += (i+1) * index * lens.lens;
        index++;
      }
    }
  }
  cout << "result: " << result << endl;
}

int main() {
  read();
  second();
}

