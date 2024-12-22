#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>


constexpr char nl { '\n' };
constexpr char delimiter { ' ' };

constexpr int64_t mod = 16'777'216;
constexpr int64_t one = 64;
constexpr int64_t two = 32;
constexpr int64_t three = 2048;

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << delimiter;
  });
  return out;
}

int64_t next(int64_t secret) {
  secret = ((secret * one) ^ secret) % mod;
  secret = ((secret / two) ^ secret) % mod;
  return ((secret * three) ^ secret) % mod;
}

std::array<int64_t, 4> update(std::array<int64_t, 4> difference, int64_t diff) {
  for (int index = 1; index < 4; ++index) {
    difference[index - 1] = difference[index];
  }
  difference[3] = diff;
  return difference;
}

int64_t last_digit(int64_t secret) {
  return secret % 10;
}

void first(std::vector<int64_t> secrets) {
  int64_t result = 0;
  for (int64_t secret: secrets) {
    for (int index = 0; index < 2'000; ++index) {
      secret = next(secret);
    }
    result += secret;
  }
  std::cout << result << nl;
}

void second(std::vector<int64_t> secrets) {
  int n = secrets.size();
  std::vector<std::pair<std::array<int64_t, 4>, int64_t>> changes;

  for (int buyer = 0; buyer < n; ++buyer) {
    int64_t secret = secrets[buyer];
    std::array<int64_t, 4> difference;

    std::set<std::array<int64_t, 4>> differences;

    int last = last_digit(secret);
    for (int index = 0; index < 2'000; ++index) {
      secret = next(secret);
      int current = last_digit(secret);
      difference = update(difference, current - last);

      if (!differences.contains(difference)) {
        differences.insert(difference);
        changes.push_back({ difference, current });
      }
      last = current;
    }
  }

  std::map<std::array<int64_t, 4>, int64_t> result;
  for (auto [difference, price]: changes) {
    result[difference] += price;
  }

  int64_t best = 0;
  for (auto [difference, total]: result) {
    if (total > best) best = total;
  }

  std::cout << best << nl;
}


int main() {
  std::vector<int64_t> secrets;
  int64_t secret;
  while (std::cin >> secret) {
    secrets.push_back(secret);
  }

  first(secrets);
  second(secrets);
}
