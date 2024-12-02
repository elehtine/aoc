#ifndef READ_H
#define READ_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> read_lines();

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vec);

#endif
