#include <iostream>
#include <sstream>
#include <algorithm>
#include <bit>
#include <vector>
#include <array>
#include <set>
#include <map>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}

using Pair = std::pair<int, int>;

using Set = std::set<int>;

Set merge(Set left, Set right) {
  for (int r: right) left.insert(r);
  return left;
}

Set intersection(Set left, Set right) {
  Set result;
  for (int r: right) {
    if (left.contains(r)) result.insert(r);
  }
  return result;
}

Set minus(Set left, Set right) {
  for (int r: right) left.erase(r);
  return left;
}

Set all(int n) {
  Set set;
  for (int node = 0; node < n; ++node) {
    set.insert(node);
  }
  return set;
}

// Bron-Kerbosch
std::vector<Set> cliques_with_pivoting(Set clique, Set possible, Set exclude, const std::vector<Set>& adjacency) {
  if (possible.empty() && exclude.empty()) return { clique };

  std::vector<Set> result;

  int pivot = *merge(possible, exclude).begin();
  while (!minus(possible, adjacency[pivot]).empty()) {
    int node = *minus(possible, adjacency[pivot]).begin();

    Set new_possible = intersection(possible, adjacency[node]);
    Set new_exclude = intersection(exclude, adjacency[node]);
    auto found = cliques_with_pivoting(merge(clique, { node }), new_possible, new_exclude, adjacency);

    result.insert(result.end(), found.begin(), found.end());

    possible.erase(node);
    exclude.insert(node);
  }

  return result;
}

std::vector<Set> cliques(const std::vector<Set>& adjacency, int n) {
  Set possible = all(n);
  Set exclude;
  std::vector<int> nodes(possible.begin(), possible.end());
  std::ranges::sort(nodes, [adjacency](int left, int right) {
    return adjacency[left].size() < adjacency[right].size();
  });

  std::vector<Set> result;
  for (int node: nodes) {
    Set new_possible = intersection(possible, adjacency[node]);
    Set new_exclude = intersection(exclude, adjacency[node]);
    auto found = cliques_with_pivoting({ node }, new_possible, new_exclude, adjacency);
    result.insert(result.end(), found.begin(), found.end());

    possible.erase(node);
    exclude.insert(node);
  }
  return result;
}

void first(const std::vector<Set>& adjacency, std::map<int, std::string> int_to_string, int n) {
  std::set<Set> triplets;
  for (int one: all(n)) {
    for (int two: adjacency[one]) {
      for (int three: adjacency[two]) {
        for (int four: adjacency[three]) {
          if (four != one) continue;
          triplets.insert({ one, two, three });
        }
      }
    }
  }

  int result = 0;
  for (Set triplet: triplets) {
    bool success = false;
    for (int computer: triplet) {
      if (int_to_string[computer][0] == 't') success = true;
    }
    if (success) ++result;
  }
  std::cout << result << nl;
}

void second(const std::vector<Set>& adjacency, std::map<int, std::string> int_to_string, int n) {
  std::vector<Set> found = cliques(adjacency, n);
  int max = 0;
  for (auto clique: found) {
    if (max < std::ssize(clique)) max = clique.size();
  }

  std::vector<std::string> names;
  for (auto clique: found) {
    if (std::ssize(clique) == max) {
      for (int computer: clique) {
        names.push_back(int_to_string[computer]);
      }
      break;
    }
  }

  std::ranges::sort(names);
  std::string result { "" };
  for (std::string name: names) {
    result += "," + name;
  }
  std::cout << result.substr(1) << nl;
}


int main() {
  std::vector<Set> adjacency;

  std::map<std::string, int> string_to_int;
  std::map<int, std::string> int_to_string;
  int count = 0;

  std::string line;
  while (std::getline(std::cin, line)) {
    for (char& c: line) {
      if (c == '-') c = ' ';
    }

    std::istringstream stream { line };
    std::string from, to;
    stream >> from >> to;

    for (std::string computer: { from, to }) {
      if (!string_to_int.contains(computer)) {
        string_to_int[computer] = count;
        int_to_string[count] = computer;
        adjacency.push_back({});
        ++count;
      }
    }

    adjacency[string_to_int[from]].insert(string_to_int[to]);
    adjacency[string_to_int[to]].insert(string_to_int[from]);
  }

  first(adjacency, int_to_string, count);
  second(adjacency, int_to_string, count);
}
