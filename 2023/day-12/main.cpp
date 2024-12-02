#include <iostream>
#include <vector>
#include <string>

#include "readwrite.h"

using namespace std;

struct Spring {
  string data;
  vector<int> groups;
};

ostream& operator<<(ostream& out, const Spring& spring) {
  out << spring.data << endl;
  out << spring.groups << endl;
  return out;
}

vector<Spring> springs;

void read() {
  bool second = true;
  vector<string> lines = read_lines();
  for (string line: lines) {
    vector<string> parts = split(line, " ");
    string data = parts[0];
    vector<int> groups = toint(split(parts[1], ","));
    if (second) {
      data = data+"?"+data+"?"+data+"?"+data+"?"+data;
      groups = groups + groups + groups + groups + groups;
    }
    springs.push_back({ data, groups });
  }
}

long count(
    Spring spring,
    int index,
    int group,
    bool skip,
    vector<vector<long>>& table
) {
  int n = spring.data.size();
  int m = spring.groups.size();
  table[n][m] = 1;

  for (int index = n-1; index >= 0; index--) {
    const char current = spring.data[index];
    for (int group = m; group >= 0; group--) {
      if (group == m) {
        if (current == '#') table[index][group] = 0;
        else table[index][group] = table[index+1][group];
        continue;
      }

      if (current != '.') {
        int add = spring.groups[group];
        bool correct = false;
        if (index + add <= n) {
          correct = true;
          for (int d = 0; d < add; d++) {
            if (spring.data[index + d] == '.') correct = false;
          }
          if (index + add != n && spring.data[index + add] == '#') {
            correct = false;
          }
        }
        if (!correct) {
        } else if (index+add == n) {
          table[index][group] += table[index+add][group+1];
        } else {
          table[index][group] += table[index+add+1][group+1];
        }
      }

      if (current != '#') {
        table[index][group] += table[index+1][group];
      }
    }
  }

  /*
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= m; j++) {
      cout << table[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  */

  return table[0][0];
}

void solve() {
  cout << springs << endl;
  long result = 0;
  for (const Spring& spring: springs) {
    int n = spring.data.size() + 1;
    int m = spring.groups.size() + 1;
    vector<vector<long>> table =
      vector<vector<long>>(n, vector<long>(m, 0));
    long c = count(spring, 0, 0, false, table);
    result += c;
    cout << c << " ";
  }
  cout << endl;
  cout << result << endl;
}

int main() {
  read();
  solve();
}

