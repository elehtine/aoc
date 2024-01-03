#include <iostream>
#include <memory>
#include <map>

#include "readwrite.h"

using namespace std;

struct Signal {
  string from, to;
  bool low;
};

ostream& operator<<(ostream& out, const Signal& signal) {
  out << signal.from;
  if (signal.low) out << " -low-> ";
  else out << " -high-> ";
  out << signal.to;
  return out;
}

struct Module {
  string name;
  vector<string> cables;

  Module() {}

  Module(string name, vector<string> cables):
    name(name), cables(cables) {}

  string to_string() const {
    string result = type();
    result += name;
    result += " -> ";
    bool comma = false;
    for (string cable: cables) {
      if (comma) result += ", ";
      comma = true;
      result += cable;
    }
    return result;
  }

  virtual string type() const = 0;
  virtual void handle(const Signal& signal) = 0;
  virtual void save(const Module& mod) = 0;
};


vector<unique_ptr<Module>> modules;
vector<Signal> signals;
vector<Signal> start;

struct Flip : public Module {
  bool on = false;

  Flip(string name, vector<string> cables): Module { name, cables } {};

  string type() const override {
    return "% ";
  }

  void handle(const Signal& signal) override {
    if (!signal.low) return;
    on = !on;
    for (const string& s: cables) {
      signals.push_back({ name, s, !on });
    }
  }
  void save(const Module& mod) override {}
};

struct Conjunction : public Module {
  map<string, bool> remember;

  Conjunction(string name, vector<string> cables): 
    Module { name, cables } {}

  string type() const override {
    return "& ";
  }

  void handle(const Signal& signal) override {
    remember[signal.from] = signal.low;
    bool high = true;
    for (auto input: remember) {
      if (input.second) high = false;
    }

    for (const string& s: cables) {
      signals.push_back({ name, s, high });
    }
  }

  void save(const Module& mod) override {
    for (string cable: mod.cables) {
      if (cable == name) remember[mod.name] = true;
    }
  }
};

ostream& operator<<(ostream& out, const Module& mod) {
  out << mod.to_string();
  return out;
}

void read() {
  vector<string> lines = read_lines();
  for (string line: lines) {
    vector<string> parts = split(line, " -> ");
    string name = parts[0].substr(1);
    vector<string> cables = split(parts[1], ", ");
    if (parts[0] == "broadcaster") {
      for (string s: cables) {
        start.push_back({ "broadcaster", s, true });
      }
    } else if (parts[0][0] == '&') {
      modules.push_back(make_unique<Conjunction>(name, cables));
    } else {
      modules.push_back(make_unique<Flip>(name, cables));
    }
  }
}

void first() {
  const int count = 1000;
  int low = 0;
  int high = 0;
  for (unique_ptr<Module>& one: modules) {
    for (unique_ptr<Module>& other: modules) {
      one->save(*other);
    }
  }

  for (int i = 0; i < count; i++) {
    low++;
    signals = start;

    while (!signals.empty()) {
      Signal signal = signals.front();
      signals.erase(signals.begin());
      if (signal.low) low++;
      else high++;

      if (signal.to == "output") continue;

      for (unique_ptr<Module>& mod: modules) {
        if (mod->name != signal.to) continue;
        mod->handle(signal);
        break;
      }
    }
  }
  cout << "low: " << low << endl;
  cout << "high: " << high << endl;
  cout << "result: " << low * high << endl;
}

int main() {
  read();
  first();
}
