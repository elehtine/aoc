#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Jump {
  long first, add;
};

string instructions;
vector<string> positions;
map<string, pair<string, string>> network;
map<int, Jump> jumps;

void read() {
  cin >> instructions;
  string line;
  getline(cin, line);
  getline(cin, line);

  string name, left, right;
  while (getline(cin, line)) {
    name = line.substr(0, 3);
    left = line.substr(7, 3);
    right = line.substr(12, 3);
    network[name] = { left, right };

    if (name[2] == 'A') positions.push_back(name);
  }
}

bool check() {
  for (int index = 0; index < (int) positions.size(); index++) {
    if (jumps[index].add == -1) return false;
  }
  return true;
}

long gcd(long a, long b) {
  if (a == b) return a;
  if (a < b) return gcd(b, a);
  int c = (a - 1) / b;
  return gcd(a - b*c, b);
}

void solve() {
  for (int index = 0; index < (int) positions.size(); index++) {
    jumps[index] = Jump { -1, -1 };
  }

  for (int counter = 0; true; counter++) {
    char dir = instructions[counter % instructions.size()];
    if (dir == 'L') {
      for (string& pos: positions) {
        pos = network[pos].first;
      }
    } else {
      for (string& pos: positions) {
        pos = network[pos].second;
      }
    }

    for (int index = 0; index < (int) positions.size(); index++) {
      if (positions[index][2] == 'Z') {
        if (jumps[index].first == -1) jumps[index].first = counter + 1;
        else if (jumps[index].add == -1) {
          jumps[index].add = counter + 1 - jumps[index].first;
        }
      }
    }

    if (check()) {
      break;
    }
  }

  for (int index = 0; index < (int) positions.size(); index++) {
    cout << jumps[index].first << " " << jumps[index].add << endl;
  }


  long first = jumps[0].first;
  long add = jumps[0].add;
  for (int index = 1; index < (int) positions.size(); index++) {
    long counter = first;
    cout << "first: " << first << " add: " << add << endl;
    while (true) {
      long ring = counter - jumps[index].first;
      if (ring % jumps[index].add == 0) {
        first = counter;
        add /= gcd(add, jumps[index].add);
        add *= jumps[index].add;
        break;
      }
      counter += add;
    }
  }
  cout << "first: " << first << " add: " << add << endl;

  cout << "result: " << first << endl;
}

int main() {
  read();
  solve();
}

