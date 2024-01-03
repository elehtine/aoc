#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Range {
  long dest, source, length;
};

struct Convert {
  string from, to;
  vector<Range> ranges;
};

vector<string> lines;
vector<long> seeds;
vector<Convert> conversions;

template<typename T>
ostream& operator<<(ostream& output, const vector<T> v) {
  for (const T& value: v) {
    output << value << endl;
  }
  return output;
}

ostream&operator<<(ostream& output, const Range& range) {
  output << range.dest << " " << range.source << " " << range.length;
  return output;
}

ostream&operator<<(ostream& output, const Convert& convert) {
  output << convert.from << " " << convert.to << ":" << endl;
  output << convert.ranges;
  return output;
}

void read_nums(vector<long>& v, string line) {
  long num = 0;
  for (char c: line) {
    if (c == ' ') {
      v.push_back(num);
      num = 0;
      continue;
    }

    num *= 10;
    num += c - '0';
  }
  if (num != 0) v.push_back(num);
}

void read_seeds() {
  const string line = lines[0];
  read_nums(seeds, line.substr(7));
}

void read_ranges() {
  string from = "", to = "";
  vector<Range> ranges;
  for (size_t index = 2; index < lines.size(); index++) {
    string line = lines[index];
    if (from == "") {
      long dash = line.find("-");
      long space = line.find(" ");
      from = line.substr(0, dash);
      to = line.substr(dash + 4, space - dash - 4);
      continue;
    }
    if (line == "") {
      Convert convert { from, to, ranges };
      ranges.clear();

      conversions.push_back(convert);
      from = "";
      continue;
    }

    vector<long> nums;
    read_nums(nums, line);
    ranges.push_back(Range { nums[0], nums[1], nums[2] });
  }

  Convert convert { from, to, ranges };
  conversions.push_back(convert);
}

void read() {
  string s;
  while (getline(cin, s)) {
    lines.push_back(s);
  }

  read_seeds();
  read_ranges();
}

pair<long, long> do_conversion(long seed) {
  long shortest = -1;
  for (Convert convert: conversions) {
    for (Range range: convert.ranges) {
      if (range.source <= seed && seed < range.source + range.length) {
        long r = range.source + range.length - seed;
        seed += range.dest - range.source;
        if (shortest == -1) shortest = r;
        shortest = min(shortest, r);
        break;
      }
    }
  }
  return make_pair(seed, shortest);
}

void solve() {
  long result = -1;
  for (int index = 0; index < (int) seeds.size() / 2; index++) {
    long seed = seeds[index * 2];
    long len = seeds[index * 2 + 1];

    while (len > 0) {
      pair<long, long> res = do_conversion(seed);
      long value = res.first;
      long r = res.second;
      if (result == -1) {
        result = value;
      }
      result = min(result, value);

      seed = seed + r;
      len = len - r;
    }
  }
  cout << result << endl;
}

int main() {
  read();
  solve();
}
