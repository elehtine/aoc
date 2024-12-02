#include <iostream>

#include "readwrite.h"

using namespace std;

enum class Type {
  flip, conjunction, broadcaster
};

Type type(const string& s) {
  if (s[0] == '%') return Type::flip;
  if (s[0] == '&') return Type::conjunction;
  return Type::broadcaster;
}

ostream& operator<<(ostream& out, const Type type) {
  if (type == Type::flip) out << "%";
  if (type == Type::conjunction) out << "&";
  return out;
}

struct Module {
  Type type;
  string name;
  vector<string> from;
  vector<string> to;
  bool on;
  vector<bool> remember;
};

Module parse_module(const string& line) {
  string before = split(line, " -> ")[0];
  string after = split(line, " -> ")[1];
  string name = before == "broadcaster" ? before : before.substr(1);
  return { type(before), name, {}, { split(after, ", ") }, false, {} };
}

string join(const vector<string>& parts) {
  string result = "[";
  bool comma = false;
  for (const string& part: parts) {
    if (comma) result += ",";
    comma = true;
    result += " ";
    result += part;
  }
  result += " ]";
  return result;
}

ostream& operator<<(ostream& out, const Module& mod) {
  if (mod.from.empty()) out << "button";
  else {
    vector<string> from;
    for (int index = 0; index < (int) mod.from.size(); index++) {
      string mark = mod.remember[index] ? " +" : " -";
      from.push_back(mod.from[index] + mark);
    }
    out << join(from);
  }
  out << " -> ";
  out << mod.type;
  out << mod.name;
  if (mod.type == Type::flip) out << (mod.on ? "+" : "-");
  out << " -> ";
  out << join(mod.to);
  return out;
}

struct Signal {
  bool high;
  string from, to;
};

ostream& operator<<(ostream& out, const Signal& signal) {
  out << signal.from;
  if (signal.high) out << " -high-> ";
  else out << " -low-> ";
  out << signal.to;
  return out;
}

vector<Module> modules;
vector<Signal> signals;

int find_module(const string& name) {
  for (int index = 0; index < (int) modules.size(); index++) {
    if (modules[index].name == name) return index;
  }
  return -1;
}

void read() {
  vector<string> lines = read_lines();
  for (const string& line: lines) {
    modules.push_back(parse_module(line));
  }
  for (const Module& mod: modules) {
    for (const string& receiver: mod.to) {
      int index = find_module(receiver);
      if (index == -1) continue;
      modules[index].from.push_back(mod.name);
      modules[index].remember.push_back(false);
    }
  }
}

bool handle_signal(const Signal& signal) {
  bool found = false;
  if (signal.to == "qt" && signal.high) {
    found = true;
  }
  int index = find_module(signal.to);
  if (index == -1) return found;

  Module& mod = modules[index];
  bool high = false;
  if (mod.type == Type::flip) {
    if (signal.high) return found;
    mod.on = !mod.on;
    high = mod.on;
  } else if (mod.type == Type::conjunction) {
    for (int index = 0; index < (int) mod.from.size(); index++) {
      if (mod.from[index] == signal.from) mod.remember[index] = signal.high;
    }
    for (const bool& last: mod.remember) {
      if (!last) high = true;
    }
  }

  for (const string& to: mod.to) {
    signals.push_back({ high, signal.to, to });
  }
  return found;
}

bool button(const string& start) {
  bool found = false;
  signals.push_back({ false, "button", start});
  while (!signals.empty()) {
    Signal signal = signals.front();
    signals.erase(signals.begin());
    if (handle_signal(signal)) found = true;
  }
  return found;
}

void second() {
  long result = 1;
  int broadcaster = find_module("broadcaster");
  for (const string& start: modules[broadcaster].to) {
    for (long counter = 1; counter <= 20'000'000; counter++) {
      if (button(start)) {
        result *= counter;
        cout << "found: " << counter << endl;
        break;
      }
    }
  }
  cout << result << endl;
}

int main() {
  read();
  second();
}
