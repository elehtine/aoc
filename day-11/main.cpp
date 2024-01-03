#include <iostream>
#include <vector>

using namespace std;

vector<string> lines;

void read() {
  string line;
  while (getline(cin, line)) {
    cin >> line;
    lines.push_back(line);
  }
}

int main() {
  read();
}

