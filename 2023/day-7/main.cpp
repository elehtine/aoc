#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

struct Hand {
  string cards;
  long bid;
};

enum class Label {
  five_of_a_kind,
  four_of_a_kind,
  full_house,
  three_of_a_kind,
  two_pair,
  pair,
  high_card,
};

ostream& operator<<(ostream& output, const Label label) {
  if (label == Label::five_of_a_kind) output << "five of a kind";
  if (label == Label::four_of_a_kind) output << "four of a kind";
  if (label == Label::full_house) output << "full house";
  if (label == Label::three_of_a_kind) output << "three of a kind";
  if (label == Label::two_pair) output << "two pair";
  if (label == Label::pair) output << "pair";
  if (label == Label::high_card) output << "high card";
  return output;
}

inline bool operator< (const Label lhs, const Label rhs) {
  if (lhs == rhs) return false;
  array<Label, 7> labels {
    Label::five_of_a_kind,
    Label::four_of_a_kind,
    Label::full_house,
    Label::three_of_a_kind,
    Label::two_pair,
    Label::pair,
    Label::high_card
  };
  for (const Label& label: labels) {
    if (rhs == label) return true;
    if (lhs == label) return false;
  }
  return false;
}
inline bool operator> (const Label lhs, const Label rhs) {
  return rhs < lhs;
}
inline bool operator<=(const Label lhs, const Label rhs) {
  return !(lhs > rhs);
}
inline bool operator>=(const Label lhs, const Label rhs) {
  return !(lhs < rhs);
}

int card_index(const char& c) {
  if (c == 'A') return 14;
  if (c == 'K') return 13;
  if (c == 'Q') return 12;
  if (c == 'J') return 1;
  if (c == 'T') return 10;
  return c - '0';
}

Label hand_label(const Hand& hand) {
  array<int, 15> counts {};
  for (const char& card: hand.cards) {
    int index = card_index(card);
    counts[index]++;
  }

  bool five = false;
  bool four = false;
  bool three = false;
  bool two = false;
  int jokers = counts[1];
  for (int index = 0; index < (int) counts.size(); index++) {
    if (index == 1) continue;
    int count = counts[index];

    if (count == 5) five = true;
    if (count == 4 && jokers == 1) five = true;
    if (count == 3 && jokers == 2) five = true;
    if (count == 2 && jokers == 3) five = true;
    if (count == 1 && jokers == 4) five = true;
    if (count == 0 && jokers == 5) five = true;

    if (count == 4) four = true;
    if (count == 3 && jokers == 1) four = true;
    if (count == 2 && jokers == 2) four = true;
    if (count == 1 && jokers == 3) four = true;
    if (count == 0 && jokers == 4) four = true;

    if (count == 3) three = true;
    if (count == 2) {
      if (two == true && jokers == 1) return Label::full_house;
      if (two == true) return Label::two_pair;
      two = true;
    }
  }

  if (five) return Label::five_of_a_kind;
  if (four) return Label::four_of_a_kind;
  if (three && two) return Label::full_house;

  if (three) return Label::three_of_a_kind;
  if (two && jokers == 1) return Label::three_of_a_kind;
  if (jokers == 2) return Label::three_of_a_kind;

  if (two) return Label::pair;
  if (jokers == 1) return Label::pair;

  return Label::high_card;
}

inline bool operator<(const Hand& lhs, const Hand& rhs) {
  Label l_label = hand_label(lhs);
  Label r_label = hand_label(rhs);
  if (l_label < r_label) return true;
  if (l_label > r_label) return false;
  for (int index = 0; index < 5; index++) {
    int l_index = card_index(lhs.cards[index]);
    int r_index = card_index(rhs.cards[index]);
    if (l_index == r_index) continue;
    if (l_index < r_index) return true;
    return false;
  }
  return false;
}
inline bool operator>(const Hand& lhs, const Hand& rhs) {
  return rhs < lhs;
}
inline bool operator<=(const Hand& lhs, const Hand& rhs) {
  return !(lhs > rhs);
}
inline bool operator>=(const Hand& lhs, const Hand& rhs) {
  return !(lhs < rhs);
}

vector<Hand> hands;

istream& operator>>(istream& input, Hand& hand) {
  cin >> hand.cards;
  cin >> hand.bid;
  return input;
}

ostream& operator<<(ostream& output, Hand& hand) {
  output << hand.cards << " " << hand.bid << " ";
  return output;
}

template<typename T>
ostream& operator<<(ostream& output, vector<T> vec) {
  for (T& value: vec) {
    output << value << endl;
  }
  return output;
}

void read() {
  Hand hand;
  while (cin >> hand) {
    hands.push_back(hand);
  }
  sort(hands.begin(), hands.end());
}

void solve() {
  long k = 1;
  long result = 0;
  for (const Hand& hand: hands) {
    result += hand.bid * k;
    k++;
  }
  cout << result << endl;
}

int main() {
  read();
  solve();
}
