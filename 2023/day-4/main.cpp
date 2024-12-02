#include <iostream>
#include <vector>

using namespace std;

struct Card {
  int id;
  vector<int> win;
  vector<int> your;
};

istream& operator>>(istream& input, Card& card) {
  card.win.clear();
  card.your.clear();
  string s;
  if (!getline(input, s)) return input;
  int start = 5;
  int colon = s.find(':');
  int bar = s.find("|");

  int first_size = (bar - colon) / 3;
  int second_size = (s.size() - bar) / 3;

  card.id = stoi(s.substr(start, colon - start));
  for (int index = 0; index < first_size; index++) {
    card.win.push_back(stoi(s.substr(colon + 3*index + 1, 3)));
  }

  for (int index = 0; index < second_size; index++) {
    card.your.push_back(stoi(s.substr(bar + 3*index + 1, 3)));
  }
  return input;
}

ostream& operator<<(ostream& output, const Card& card) {
  output << "Card " << card.id << ": ";
  for (int value: card.win) {
    output << value << " ";
  }
  output << "|";
  for (int value: card.your) {
    output << " " << value;
  }
  return output;
}

template<typename T>
ostream& operator<<(ostream& output, const vector<T>& v) {
  output << "[" << endl;
  for (const T& value: v) {
    output << value << endl;
  }
  output << "]" << endl;
  return output;
}

vector<Card> cards;

void read() {
  Card card;
  while (cin >> card) {
    cards.push_back(card);
  }
}

int points(const Card& card) {
  int points = 0;

  for (int num: card.your) {
    bool found = false;
    for (int win: card.win) {
      if (num == win) found = true;
    }

    if (found) {
      points++;
    }
  }

  return points;
}

int first() {
  int sum = 0;
  for (const Card& card: cards) {
    int p = points(card);
    if (p == 0) continue;

    int num = 1;
    for (int i = 1; i < p; i++) {
      num *= 2;
    }

    sum += num;
  }

  return sum;
}

int second() {
  vector<int> result(cards.size(), 1);
  int sum = 0;
  for (size_t index = 0; index < cards.size(); index++) {
    Card card = cards[index];
    int p = points(card);
    for (int i = 1; i <= p; i++) {
      if (index + i < cards.size()) result[index + i] += result[index];
    }

    sum += result[index];
  }
  return sum;
}

void solve() {
  int sum = second();
  cout << sum << endl;
}

int main() {
  read();
  solve();
}
