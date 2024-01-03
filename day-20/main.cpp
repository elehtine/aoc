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
  if (type == Type::flip) out << "% ";
  if (type == Type::conjunction) out << "& ";
  return out;
}

struct Module {
  Type type;
  vector<string> name;
  vector<string> from;
  vector<string> to;
};

Module parse_module(const string& line) {
  string before = split(line, " -> ")[0];
  string after = split(line, " -> ")[1];
  string name = before == "broadcaster" ? before : before.substr(1);
  return { type(before), { name }, {}, { split(after, ", ") } };
}

string join(const vector<string>& parts) {
  string result = "[";
  bool comma = false;
  for (const string& part: parts) {
    if (comma) result += ",";
    result += " ";
    result += part;
  }
  result += " ]";
  return result;
}

vector<Module> modules;

int find_module(const string& name) {
  for (int index = 0; index < (int) modules.size(); index++) {
    if (modules[index].name[0] == name) return index;
  }
  return -1;
}

ostream& operator<<(ostream& out, const Module& mod) {
  int flip_from = 0;
  int conjunction_from = 0;
  int flip_to = 0;
  int conjunction_to = 0;
  for (const string& sender: mod.from) {
    int index = find_module(sender);
    if (index == -1) continue;
    if (modules[index].type == Type::flip) flip_from++;
    if (modules[index].type == Type::conjunction) conjunction_from++;
  }
  for (const string& receiver: mod.to) {
    int index = find_module(receiver);
    if (index == -1) continue;
    if (modules[index].type == Type::flip) flip_to++;
    if (modules[index].type == Type::conjunction) conjunction_to++;
  }

  out << mod.type;
  out << join(mod.name) << " ";
  out << Type::flip << flip_from << ", ";
  out << Type::conjunction << conjunction_from << " -> ";
  out << Type::flip << flip_to << ", ";
  out << Type::conjunction << conjunction_to;
  return out;
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
      modules[index].from.push_back(mod.name[0]);
    }
  }

  cout << modules << endl;
}

void first() {
  cout << "first" << endl;
}

int main() {
  read();
  first();
}
