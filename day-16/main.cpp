#include <iostream>
#include <array>

#include "readwrite.h"

using namespace std;

vector<string> tiles;
vector<vector<array<bool, 4>>> energised;
int width, height;

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

void read() {
  tiles = read_lines();
  width = tiles[0].size();
  height = tiles.size();
}

void go(int x, int y, int direction) {
  if (x < 0 || y < 0) return;
  if (x >= width || y >= height) return;

  if (energised[y][x][direction]) return;
  energised[y][x][direction] = true;

  char current = tiles[y][x];
  bool north = false;
  bool east = false;
  bool south = false;
  bool west = false;
  if (current == '.') {
    if (direction == NORTH) north = true;
    if (direction == EAST) east = true;
    if (direction == SOUTH) south = true;
    if (direction == WEST) west = true;
  }
  if (current == '|') {
    if (direction == NORTH) north = true;
    if (direction == SOUTH) south = true;
    if (direction == EAST || direction == WEST) {
      north = true;
      south = true;
    }
  }
  if (current == '-') {
    if (direction == NORTH || direction == SOUTH) {
      east = true;
      west = true;
    }
    if (direction == EAST) east = true;
    if (direction == WEST) west = true;
  }
  if (current == '/') {
    if (direction == NORTH) east = true;
    if (direction == SOUTH) west = true;
    if (direction == EAST) north = true;
    if (direction == WEST) south = true;
  }
  if (current == '\\') {
    if (direction == NORTH) west = true;
    if (direction == SOUTH) east = true;
    if (direction == EAST) south = true;
    if (direction == WEST) north = true;
  }

  if (north) go(x, y-1, NORTH);
  if (east) go(x+1, y, EAST);
  if (south) go(x, y+1, SOUTH);
  if (west) go(x-1, y, WEST);
}

int score(int x, int y, int direction) {
  energised = vector<vector<array<bool, 4>>>(
    height, vector<array<bool, 4>>(
      width, { false, false, false, false }
    )
  );
  go(x, y, direction);

  int result = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      bool energy = false;
      for (int dir = 0; dir < 4; dir++) {
        if (energised[y][x][dir]) energy = true;
      }
      if (energy) result++;
    }
  }
  return result;
}

void first() {
  cout << score(0, 0, EAST) << endl;
}

void second() {
  int best = 0;
  for (int x = 0; x < width; x++) {
    best = max(best, score(x, 0, SOUTH));
    best = max(best, score(x, height-1, NORTH));
  }
  for (int y = 0; y < height; y++) {
    best = max(best, score(0, y, EAST));
    best = max(best, score(width-1, y, WEST));
  }
  cout << best << endl;
}

int main() {
  read();
  second();
}

