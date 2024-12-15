#include <iostream>
#include <algorithm>
#include <numeric>
#include <regex>
#include <vector>
#include <set>
#include <map>


constexpr char nl { '\n' };

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  std::ranges::for_each(vector, [&out](const T& element) {
    out << element << nl;
  });
  return out;
}

using Point = std::pair<int, int>;

Point fail = { -1, -1 };

struct Robot {
  Point position, velocity;
};

auto compute_update(int width, int height) {
  auto update = [=](Point& position, Point velocity) -> void {
    auto& [x, y] = position;
    auto [dx, dy] = velocity;
    x = ((x + dx) % width + width) % width;
    y = ((y + dy) % height + height) % height;
  };

  return update;
}

Point dimensions(std::vector<Robot> robots) {
  int width = 11;
  int height = 7;

  for (Robot robot: robots) {
    if (robot.position.first > width) {
      width = 101;
      height = 103;
    }
  }

  return { width, height };
}


void first(std::vector<Robot> robots) {
  auto [width, height] = dimensions(robots);

  int one = 0;
  int two = 0;
  int three = 0;
  int four = 0;

  for (Robot robot: robots) {
    robot.position.first += robot.velocity.first * 100;
    robot.position.second += robot.velocity.second * 100;
    robot.position.first = (robot.position.first % width + width) % width;
    robot.position.second = (robot.position.second % height + height) % height;

    if (robot.position.first < width / 2) {
      if (robot.position.second < height / 2) ++one;
      if (robot.position.second > height / 2) ++two;
    }
    if (robot.position.first > width / 2) {
      if (robot.position.second < height / 2) ++three;
      if (robot.position.second > height / 2) ++four;
    }
  }

  std::cout << one * two * three * four << nl;
}

void second(std::vector<Robot> robots) {
  auto [width, height] = dimensions(robots);
  int size = robots.size();
  auto update = compute_update(width, height);

  std::vector<Point> points;
  std::vector<Point> velocities;

  for (int index = 0; index < std::min(500, size); ++index) {
    auto [position, velocity] = robots[index];
    points.push_back(position);
    velocities.push_back(velocity);
  }

  int max = 0;
  int best = 0;
  for (int step = 1; step <= width*height; ++step) {
    for (int index = 0; index < std::ssize(points); ++index) {
      update(points[index], velocities[index]);
    }

    std::map<int, int> counts;
    int line = 0;
    for (Point point: points) {
      auto [x, y] = point;
      int current_line = x - y;
      ++counts[current_line];
      if (counts[current_line] > counts[line]) {
        line = current_line;
      }
    }

    if (counts[line] > max) {
      max = counts[line];
      best = step;
    }
  }

  std::vector<std::string> map(height, std::string(width, '.'));
  for (Robot robot: robots) {
    auto [position, velocity] = robot;
    velocity.first *= best;
    velocity.second *= best;
    update(position, velocity);
    map[position.second][position.first] = '#';
  }

  std::cout << map << nl;
  std::cout << best << nl;
}

Robot parse(std::string line) {
  std::regex number_regex(R"(-?\d+)");
  Robot result;

  auto begin = std::sregex_iterator(line.begin(), line.end(), number_regex);
  auto end = std::sregex_iterator {};

  std::vector<int> numbers;
  for (std::sregex_iterator it = begin; it != end; ++it) {
    std::smatch match = *it;
    numbers.push_back(std::stoi(match.str()));
  }

  result.position.first = numbers[0];
  result.position.second = numbers[1];
  result.velocity.first = numbers[2];
  result.velocity.second = numbers[3];
  return result;
}

int main() {
  std::vector<Robot> robots;

  std::string line;
  while (std::getline(std::cin, line)) {
    robots.push_back(parse(line));
  }

  first(robots);
  second(robots);
}
