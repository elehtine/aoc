#include <iostream>
#include <vector>

using namespace std;

vector<int> times;
vector<int> distances;

ostream& operator<<(ostream& output, vector<int> v) {
  output << "[ ";
  bool comma = false;
  for (int x: v) {
    if (comma) output << ", ";
    comma = true;
    output << x;
  }
  output << " ]";
  return output;
}

int to_int(string s) {
  int result;
  try {
    result = stoi(s);
  } catch (const invalid_argument& ex) {
    result = -1;
  }
  return result;
}

void read() {
  string value;
  cin >> value;
  while (cin >> value) {
    int x = to_int(value);
    if (x == -1) break;
    times.push_back(x);
  }
  while (cin >> value) {
    distances.push_back(to_int(value));
  }
}

long search(long time, long distance) {
  long x = 0;
  for (long b = time; b >= 1; b/= 2) {
    while (x + b <= time/2 && (x+b) * (time-x-b) <= distance) {
      x += b;
    }
  }
  return x+1;
}

void solve_first() {
  int result = 1;
  for (int index = 0; index < (int) times.size(); index++) {
    int x = search(times[index], distances[index]);
    result *= times[index] - 2*x + 1;
  }
  cout << result << endl;
}

void solve() {
  long time = 0;
  long distance = 0;
  for (long x: times) {
    for (long k = 1; k <= x; k *= 10) {
      time *= 10;
    }
    time += x;
  }
  for (long x: distances) {
    for (long k = 1; k <= x; k *= 10) {
      distance *= 10;
    }
    distance += x;
  }
  long x = search(time, distance);
  cout << time - 2*x + 1 << endl;
}

int main() {
  read();
  solve();
}

