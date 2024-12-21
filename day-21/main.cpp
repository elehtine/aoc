#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
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
constexpr Pair fail { -1, -1 };

std::map<char, int> char_to_int {
  { '>', 0 },
  { '<', 1 },
  { '^', 2 },
  { 'v', 3 },
  { 'A', 4 },
};

std::map<int, char> int_to_char {
  { 0, '>' },
  { 1, '<' },
  { 2, '^' },
  { 3, 'v' },
  { 4, 'A' },
};

const std::vector<std::string> numeric_keypad { "789", "456", "123", " 0A", };
const std::vector<std::string> directional_keypad { " ^A", "<v>" };

Pair get(std::vector<std::string> keypad, char c) {
  int width = keypad[0].size();
  int height = keypad.size();
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (keypad[y][x] == c) return { x, y };
    }
  }

  return fail;
}

Pair get(std::vector<std::string> keypad, int i) {
  return get(keypad, int_to_char[i]);
}

std::vector<std::string> dfs(std::vector<std::string> keypad, std::string path, Pair from, Pair to) {
  auto [x, y] = from;

  if (keypad[y][x] == ' ') return {};
  if (from == to) return { path + 'A' };

  std::vector<std::string> result;
  auto add = [&](std::vector<std::string> next) {
    result.insert(result.end(), next.begin(), next.end());
  };

  if (x < to.first) add(dfs(keypad, path + '>', { x + 1, y }, to));
  if (x > to.first) add(dfs(keypad, path + '<', { x - 1, y }, to));
  if (y < to.second) add(dfs(keypad, path + 'v', { x, y + 1 }, to));
  if (y > to.second) add(dfs(keypad, path + '^', { x, y - 1 }, to));

  return result;
}

std::vector<std::string> numeric_paths(std::string code) {
  std::vector<std::string> current { "" };

  Pair from = get(numeric_keypad, 'A');
  for (char c: code) {
    Pair to = get(numeric_keypad, c);

    std::vector<std::string> next;
    for (std::string end: dfs(numeric_keypad, "", from, to)) {
      for (std::string start: current) {
        next.push_back(start + end);
      }
    }

    current = next;
    next.clear();
    from = to;
  }

  return current;
}


std::vector<std::vector<std::vector<int64_t>>> compute_distances() {
  int n = 5;
  int m = 26;
  std::vector<std::vector<std::vector<int64_t>>> distances(n, std::vector<std::vector<int64_t>>(n, std::vector<int64_t>(m)));
  std::vector<std::vector<std::vector<std::string>>> paths(n, std::vector<std::vector<std::string>>(n));

  for (int from = 0; from < n; ++from) {
    for (int to = 0; to < n; ++to) {
      distances[from][to][0] = 1;
    }
  }

  for (int steps = 1; steps < m; ++steps) {
    for (int from = 0; from < n; ++from) {
      for (int to = 0; to < n; ++to) {
        Pair pair_from = get(directional_keypad, from);
        Pair pair_to = get(directional_keypad, to);
        std::vector<std::string> paths = dfs(directional_keypad, "", pair_from, pair_to);

        for (std::string path: paths) {
          int64_t length = 0;

          int current = char_to_int['A'];
          for (char c: path) {
            int next = char_to_int[c];
            length += distances[current][next][steps - 1];
            current = next;
          }

          if (distances[from][to][steps] == 0 || distances[from][to][steps] > length) {
            distances[from][to][steps] = length;
          }
        }
      }
    }
  }

  return distances;
}

int64_t shortest_path(std::string code, int steps, auto distances) {
  int64_t result = 0;
  int from = char_to_int['A'];
  for (char c: code) {
    int to = char_to_int[c];
    result += distances[from][to][steps];
    from = to;
  }

  return result;
}

int64_t complexity(int64_t length, std::string code) {
  std::istringstream stream { code };
  int64_t number;
  stream >> number;
  return length * number;
}

void solve(std::vector<std::string> lines, int steps) {
  auto distances = compute_distances();

  int64_t result = 0;
  for (std::string line: lines) {
    int64_t shortest = -1;
    for (std::string code: numeric_paths(line)) {
      int64_t length = shortest_path(code, steps, distances);
      if (shortest == -1 || length < shortest) shortest = length;
    }

    result += complexity(shortest, line);
  }
  std::cout << result << nl;
}

void second(std::vector<std::string> lines) {
}


int main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }

  solve(lines, 2);
  solve(lines, 25);
}
