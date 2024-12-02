#include <iostream>

#include "readwrite.h"

using namespace std;

vector<string> read_lines() {
  vector<string> result;
  for (string line; getline(cin, line); result.push_back(line)) {}
  return result;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vec) {
  for (const T& value: vec) {
    out << value << endl;
  }
  return out;
}
