#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <array>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << ' ';
  });
  return out;
}

void first(std::vector<int64_t> sizes) {
  std::vector<int> filesystem;
  for (int64_t block = 0; block < std::ssize(sizes); ++block) {
    for (int index = 0; index < sizes[block]; ++index) {
      if (block % 2 == 0) {
        filesystem.push_back(block / 2);
      } else {
        filesystem.push_back(std::ssize(sizes) / 2);
        --sizes[std::ssize(sizes) - 1];
        if (sizes[std::ssize(sizes) - 1] == 0) {
          sizes.pop_back();
          sizes.pop_back();
        }
      }
    }
  }

  int64_t result = 0;
  for (int64_t block = 0; block < std::ssize(filesystem); ++block) {
    result += block * filesystem[block];
  }

  std::cout << result << nl;
}

void second(std::vector<int64_t> sizes) {
  std::map<int, std::pair<int64_t, int64_t>> filesystem; // block -> (id, count)
  std::array<std::set<int>, 10> free; // size -> block

  int block = 0;
  std::vector<int> inverse;
  for (int64_t id = 0; id < std::ssize(sizes); ++id) {
    if (id % 2 == 0) {
      filesystem[block] = { id / 2 , sizes[id] };
      inverse.push_back(block);
    } else { 
      free[sizes[id]].insert(block);
    }

    block += sizes[id];
  }

  std::for_each(inverse.rbegin(), inverse.rend(), [&](int block) {
    auto [id, count] = filesystem[block];
    int left_size = -1;
    int left_block = -1;

    for (int size = count; size < 10; ++size) {
      if (!free[size].empty() && *free[size].begin() < block) {
        if (left_block == -1 || *free[size].begin() < left_block) {
          left_block = *free[size].begin();
          left_size = size;
        }
      }
    }

    if (left_block == -1) return;

    int new_block = *free[left_size].begin();
    free[left_size].erase(free[left_size].begin());
    free[left_size - count].insert(new_block + count);

    filesystem.erase(block);
    filesystem[new_block] = { id, count };
  });

  int64_t result = 0;
  for (auto [block, value]: filesystem) {
    auto [id, count] = value;
    for (int64_t add = 0; add < count; ++add) {
      result += (block + add) * id;
    }
  }

  std::cout << result << nl;
}

int main() {
  std::string line;
  std::cin >> line;

  std::vector<int64_t> sizes;
  for (char c: line) {
    sizes.push_back(c - '0');
  }

  first(sizes);
  second(sizes);
}
