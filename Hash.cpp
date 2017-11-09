#include "Hash.h"
#include <iostream>
#include <random>

using namespace std;

int main() {
	srand(time(nullptr));

	Hash table(20000);

	int n;
	string str;

	for (int j = 0; j < 20; ++j) {
		for (int i = 0; i < 20000 - j * 1000; ++i) {
			n = 1 + rand() % 100;

			for (int k = 0; k < n; ++k)
				str += 1 + rand() % 254;

			table[str] = true;
			str.clear();
		}
		cout << table.getCollisions() << endl;
		table.clear();
	}
}