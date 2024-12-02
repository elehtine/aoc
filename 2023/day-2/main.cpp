#include <iostream>
#include <vector>
#include <array>

using namespace std;

struct Round {
  int red, green, blue;
};

struct Game {
  int id;
  vector<Round> rounds;
};

int game_id(const string& s) {
  int colon = s.find(':');
  int start = 5;
  string id = s.substr(start, start - colon);
  return std::stoi(id);
}

Round round_colors(string s) {
  vector<string> colors {
    { "red" },
    { "green" },
    { "blue" },
  };

  Round round { 0, 0, 0 };
  while (s.size() > 0) {
    int comma = s.find(',');
    if (comma == s.npos) comma = s.size();
    string current = s.substr(0, comma);
    s = s.substr(min((int) s.size(), comma + 2));

    int space = current.find(' ');
    for (string color: colors) {
      if (current.size() >= color.size() && current.substr(space + 1, color.size()) == color) {
        if (color == "red") round.red = stoi(current);
        if (color == "green") round.green = stoi(current);
        if (color == "blue") round.blue = stoi(current);
      }
    }
  }
  return round;
}

vector<Round> game_rounds(string s) {
  vector<Round> rounds;
  int colon = s.find(':');
  s = s.substr(colon + 2);
  while (s.size() > 0) {
    int semicolon = s.find(';');
    if (semicolon == s.npos) semicolon = s.size();
    string current = s.substr(0, semicolon);
    rounds.push_back(round_colors(current));

    s = s.substr(min((int) s.size(), semicolon + 2));
  }
  return rounds;
}

istream& operator>>(istream& input, Game& game) {
  string s;
  if (getline(input, s)) {
    game.id = game_id(s);
    game.rounds = game_rounds(s);
  }
  return input;
}

ostream& operator<<(ostream& output, const Game& game) {
  output << "Game " << game.id << ": ";
  bool comma = false;
  for (const Round& round: game.rounds) {
    if (round.red != 0) {
      if (comma) output << ", ";
      comma = true;
      output << round.red << " red";
    }
    if (round.green != 0) {
      if (comma) output << ", ";
      comma = true;
      output << round.green << " green";
    }
    if (round.blue != 0) {
      if (comma) output << ", ";
      comma = true;
      output << round.blue << " blue";
    }
  }
  return output;
}


vector<Game> games;
vector<int> results;

void read() {
  Game game;
  while (cin >> game) {
    games.push_back(game);
  }
}

void solve() {
  for (const Game& game: games) {
    int red = 0, green = 0, blue = 0;
    for (const Round& round: game.rounds) {
      red = max(round.red, red);
      green = max(round.green, green);
      blue = max(round.blue, blue);
    }
    results.push_back(red * green * blue);
  }

  int sum = 0;
  for (int result: results) {
    sum += result;
  }
  cout << sum << endl;
}

int main() {
  read();
  solve();
}
