#include <iostream>
#include <vector>
#include <string>

#include "readwrite.h"

using namespace std;

vector<string> read_lines() {
  vector<string> result;
  for (string line; getline(cin, line); result.push_back(line)) {}
  return result;
}

vector<string> split(string line, string sep) {
  for (int index = 0; index < (int) line.size(); index++) {
    if (line.substr(index, sep.size()) == sep) {
      vector<string> first = { line.substr(0, index) };
      return first + split(line.substr(index + sep.size()), sep);
    }
  }
  return { line };
}

vector<int> toint(const vector<string>& vec) {
  vector<int> result;
  for (const string& s: vec) {
    result.push_back(stoi(s));
  }
  return result;
}
