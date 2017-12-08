#pragma once

#include <deque>
#include <fstream>
#include <ostream>
#include <string>

class Tree {
private:
  Tree *L;
  int *val;
  Tree *R;

public:
  Tree() : L(nullptr), R(nullptr), val(nullptr){};

  void insert(int);
  void saveToFile(std::string);
  bool search(int);
  static bool searchInFile(std::string, int);

  friend std::ostream &operator<<(std::ostream &, Tree &);

  ~Tree();
};
