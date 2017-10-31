#include <forward_list>
#include <string>
#include <vector>
#include <tuple>
#include <ostream>
#include <iomanip>

class Hash {
private:
	int n;
	int collisions;
	std::vector<std::pair<std::string, bool>> table;

private:
	unsigned int hash(std::string str) {
		int len = str.size();

		unsigned int out = 0;
		for (int i = 0; i < len; ++i)
			out += out ^ str[i];

		return out;
	}

public:
	Hash(int n_): n(n_), collisions(0), table(n) {};

	bool& operator[](std::string str) {
		int ind = hash(str) % n;
		int off = 0;
		while (off != n && !table[ind].first.empty()) {
			++off;
			ind = (++ind) % n;
		}
		collisions += off;
		if (off != n )
			table[ind].first = str;
		return table[ind].second;
	}

	int getCollisions() {return collisions;	}

	friend std::ostream& operator<<(std::ostream& strm, Hash& tbl) {
		int c = -1;
		for (auto& i : tbl.table) {
			c++;
			if (!i.first.empty()) {
				strm << c << ": " << std::endl;

				strm << std::setw(4) << "[" << i.first << "] = " << i.second << std::endl;

				strm << std::endl;
			}
		}
		return strm;
	}
};