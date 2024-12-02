#ifndef READ_H
#define READ_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> read_lines();
vector<string> split(string line, string sep);
vector<int> toint(const vector<string>& vec);

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vec) {
  for (int index = 0; index < (int) vec.size(); index++) {
    out << vec[index] << endl;
  }
  return out;
}

template<typename T>
vector<T> operator+(const vector<T>& first, const vector<T>& second) {
  vector<T> result(first.size() + second.size());
  int n = first.size();
  for (int index = 0; index < n; index++) {
    result[index] = first[index];
  }
  for (int index = 0; index < (int) second.size(); index++) {
    result[index + n] = second[index];
  }
  return result;
}

#endif
