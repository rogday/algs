#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <deque>

class Tree {
private:
	Tree* L;
	int* val;
	Tree* R;
public:
	Tree(): L(nullptr), R(nullptr), val(nullptr) {};

	void insert(int);
	void saveToFile(std::string);
	bool search(int);
	static bool searchInFile(std::string, int);

	friend std::ostream& operator<<(std::ostream&, Tree&);

	~Tree();
};