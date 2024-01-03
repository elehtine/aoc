#include <iostream>

#include "readwrite.h"

using namespace std;

struct Part {
  int extremely;
  int musical;
  int aerodynamic;
  int shiny;

  int score(string s) const {
    if (s == "x") return extremely;
    if (s == "m") return musical;
    if (s == "a") return aerodynamic;
    if (s == "s") return shiny;
    return 0;
  }

  int total() const {
    return extremely + musical + aerodynamic + shiny;
  }
};

struct Rule {
  string feature;
  bool less;
  int threshold;
  string target;

  bool accept(const Part& part) const {
    if (less) return part.score(feature) < threshold;
    return part.score(feature) > threshold;
  }
};

struct Range {
  int start = 1;
  int end = 4000;

  bool empty() const {
    return start > end;
  }

  long score() const {
    return max(0, end - start + 1);
  }

  void accept(Rule rule) {
    if (rule.less) end = min(end, rule.threshold-1);
    else start = max(start, rule.threshold+1);
  }

  void reject(Rule rule) {
    if (rule.less) start = max(start, rule.threshold);
    else end = min(end, rule.threshold);
  }
};

ostream& operator<<(ostream& out, const Range& range) {
  out << "(";
  out << range.start << "," << range.end;
  out << ")";
  return out;
}

struct PointRange {
  string label = "in";

  Range extremely;
  Range musical;
  Range aerodynamic;
  Range shiny;

  bool empty() {
    if (extremely.empty()) return true;
    if (musical.empty()) return true;
    if (aerodynamic.empty()) return true;
    if (shiny.empty()) return true;
    return false;
  }

  long total() const {
    return extremely.score() *
      musical.score() *
      aerodynamic.score() *
      shiny.score();
  }

  PointRange accept(const Rule& rule) const {
    PointRange result = *this;
    result.label = rule.target;
    if (rule.feature == "x") result.extremely.accept(rule);
    if (rule.feature == "m") result.musical.accept(rule);
    if (rule.feature == "a") result.aerodynamic.accept(rule);
    if (rule.feature == "s") result.shiny.accept(rule);
    return result;
  }

  PointRange reject(const Rule& rule) const {
    PointRange result = *this;
    if (rule.feature == "x") result.extremely.reject(rule);
    if (rule.feature == "m") result.musical.reject(rule);
    if (rule.feature == "a") result.aerodynamic.reject(rule);
    if (rule.feature == "s") result.shiny.reject(rule);
    return result;
  }
};

ostream& operator<<(ostream& out, const PointRange& range) {
  out << range.extremely << " * ";
  out << range.musical << " * ";
  out << range.aerodynamic << " * ";
  out << range.shiny;
  return out;
}

ostream& operator<<(ostream& out, const Part& part) {
  out << "{ ";
  out << "x = " << part.extremely;
  out << ", m = " << part.musical;
  out << ", a = " << part.aerodynamic;
  out << ", s = " << part.shiny;
  out << " }";
  return out;
}

ostream& operator<<(ostream& out, const Rule& rule) {
  out << rule.feature;
  if (rule.less) out << " < ";
  else out << " > ";
  out << rule.threshold;
  out << " --> " << rule.target;
  return out;
}

struct RuleSet {
  string name;
  vector<Rule> parts;
  string target;
};

ostream& operator<<(ostream& out, const RuleSet& ruleset) {
  out << ruleset.name;
  out << " { ";
  for (Rule rule: ruleset.parts) {
    out << rule;
    out << ", ";
  }
  out << ruleset.target;
  out << " } ";
  return out;
}

vector<RuleSet> rules;
vector<Part> parts;

Rule parse_rule(string s) {
  Rule rule;
  int mark = 0;
  for (int i = 0; i < (int) s.size(); i++) {
    if (s[i] == '<') {
      rule.less = true;
      mark = i;
    }
    if (s[i] == '>') {
      rule.less = false;
      mark = i;
    }
  }

  rule.feature = s.substr(0, mark);
  vector<string> last = split(s.substr(mark+1), ":");
  rule.threshold = stoi(last[0]);
  rule.target = last[1];
  return rule;
}

RuleSet parse_ruleset(string s) {
  RuleSet set;
  int n = s.size();
  int bracket = 0;
  for (int i = 0; i < (int) s.size(); i++) {
    if (s[i] == '{') bracket = i;
  }
  set.name = s.substr(0, bracket);
  string rest = s.substr(bracket+1, n-bracket-2);
  vector<string> parts = split(rest, ",");
  for (int index = 0; index < (int) parts.size() - 1 ;index++) {
    set.parts.push_back(parse_rule(parts[index]));
  }
  set.target = parts[parts.size() - 1];
  return set;
}

Part parse_part(string s) {
  Part part;
  int n = s.size();
  vector<string> parts = split(s.substr(1, n-2), ",");
  part.extremely = stoi(parts[0].substr(2));
  part.musical = stoi(parts[1].substr(2));
  part.aerodynamic = stoi(parts[2].substr(2));
  part.shiny = stoi(parts[3].substr(2));
  return part;
}

void read() {
  vector<string> lines = read_lines();
  bool first = true;
  for (string line: lines) {
    if (line == "") {
      first = false;
      continue;
    }

    if (first) {
      rules.push_back(parse_ruleset(line));
    } else {
      parts.push_back(parse_part(line));
    }
  }
}

RuleSet find_set(string s) {
  for (const RuleSet& set: rules) {
    if (set.name == s) return set;
  }
  cout << "NOT FOUND" << endl;
  return {};
}

string process(const RuleSet& set, const Part& part) {
  for (const Rule& rule: set.parts) {
    if (rule.accept(part)) return rule.target;
  }
  return set.target;
}

void first() {
  int total = 0;
  for (const Part& part: parts) {
    string current = "in";
    while (current != "A" && current != "R") {
      cout << current << " --> ";
      current = process(find_set(current), part);
    }
    cout << current << endl;
    if (current == "A") total += part.total();
  }
  cout << total << endl;
}

long score(PointRange range, int depth) {
  if (range.empty()) return 0;
  if (range.label == "A") return range.total();
  if (range.label == "R") return 0;
  long total = 0;
  RuleSet set = find_set(range.label);
  for (const Rule& rule: set.parts) {
    total += score(range.accept(rule), depth + 1);
    range = range.reject(rule);
  }
  range.label = set.target;
  return total + score(range, depth + 1);
}

void second() {
  PointRange current;
  long points = score(current, 1);
  cout << endl;
  cout << points << endl;
}

int main() {
  read();
  second();
}
