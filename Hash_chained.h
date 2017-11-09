#include <forward_list>
#include <string>
#include <vector>
#include <tuple>
#include <ostream>
#include <iomanip>

class Hash_chained {
private:
	int n;
	int collisions;
	std::vector<std::forward_list<std::pair<std::string, bool>>> table;

private:
	unsigned int hash(std::string str) {
		int len = str.size();

		unsigned int out = 0;
		int shift = 0;

		for (int i = 0; i < len; ++i) {
			out ^= str[i] << shift;
			shift = (shift + 7) % 32;
		}

		return out;
	}

public:
	Hash_chained(int n_): n(n_), collisions(0), table(n) {};

	bool& operator[](std::string str) {
		auto& lst = table[hash(str) % n];

		for (auto& item : lst) {
			++collisions;
			if (item.first == str)
				return item.second;
		}

		lst.push_front({str, 0});
		return lst.front().second;
	}

	int getCollisions() {return collisions;	}

	void clear() {
		table.clear();
		table.resize(n);
		collisions = 0;
	}

	friend std::ostream& operator<<(std::ostream& strm, Hash_chained& tbl) {
		int c = -1;
		for (auto& i : tbl.table) {
			c++;
			if (!i.empty()) {
				strm << c << ": " << std::endl;

				for (auto& k : i)
					strm << std::setw(4) << "[" << k.first << "] = " << k.second << std::endl;

				strm << std::endl;
			}
		}
		return strm;
	}
};