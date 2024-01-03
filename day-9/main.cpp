#include <iostream>
#include <vector>

using namespace std;

vector<vector<vector<long>>> values;

void print() {
  for (const vector<vector<long>>& vec: values) {
    for (const vector<long>& v: vec) {
      for (const long& value: v) {
        cout << value << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
}

void print(const vector<vector<long>> vec) {
  for (const vector<long>& v: vec) {
    for (const long& value: v) {
      cout << value << " ";
    }
    cout << endl;
  }
}

void read() {
  string line;
  long num = 0;
  vector<long> nums;
  while (getline(cin, line)) {
    bool negative = false;
    for (const char& c: line) {
      if (c == '-') {
        negative = true;
        continue;
      }
      if (c == ' ') {
        if (negative) num = -num;
        negative = false;
        nums.push_back(num);
        num = 0;
        continue;
      }

      num *= 10;
      num += c - '0';
    }

    if (negative) num = -num;
    nums.push_back(num);
    num = 0;
    values.push_back(vector<vector<long>> { nums });
    nums.clear();
  }
}

void solve() {
  long first = 0;
  long second = 0;
  for (vector<vector<long>>& puzzle: values) {
    for (int layer = 0; layer < (int) puzzle[0].size() - 1; layer++) {
      vector<long> last = puzzle[layer];
      long n = last.size() - 1;
      vector<long> next(n);
      for (int index = 0; index < n; index++) {
        next[index] = last[index + 1] - last[index];
      }
      puzzle.push_back(next);
    }

    long current = 0;
    long s = 0;
    int n = puzzle.size() - 1;
    for (int layer = n; layer >= 0; layer--) {
      current += puzzle[layer][n-layer];
      s = puzzle[layer][0] - s;
    }

    first += current;
    second += s;
  }
  cout << "first: " << first << endl;
  cout << "second: " << second << endl;
}

int main() {
  read();
  solve();
}

