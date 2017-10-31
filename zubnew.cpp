#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <unordered_map>

using namespace std;
typedef unsigned int uint;

inline uint turn(int way, uint num) {
	if (way == 0) {
		int pos = num >> 27;

		if (pos > 5)
			return num;

		pos += 3;
		uint digit = (num & (0b111 << (pos * 3))) >> 9;
		num &= ~(0b111 << (pos * 3));

		num |= digit;
		num += 3 << 27;

		return num;
	} else if (way == 1) {
		int pos = num >> 27;

		if (pos < 3)
			return num;

		pos -= 3;
		uint digit = (num & (0b111 << (pos * 3))) << 9;
		num &= ~(0b111 << (pos * 3));

		num |= digit;
		num -= 3 << 27;

		return num;
	} else if (way == 2) {
		int pos = num >> 27;

		if (pos % 3 == 2)
			return num;

		++pos;
		uint digit = (num & (0b111 << (pos * 3))) >> 3;
		num &= ~(0b111 << (pos * 3));

		num |= digit;
		num += 1 << 27;

		return num;
	} else if (way == 3) {
		int pos = num >> 27;

		if (pos % 3 == 0)
			return num;

		--pos;
		uint digit = (num & (0b111 << (pos * 3))) << 3;
		num &= ~(0b111 << (pos * 3));

		num |= digit;
		num -= 1 << 27;

		return num;
	}
}

void print(uint num) {
	for (int i = 8; i >= 0; --i) {
		cout << ((num & (0b111 << (i * 3))) >> (i * 3)) << ' ';
		if (i % 3 == 0)
			cout << endl;
	}
	cout << endl;
}

stack<uint> func(uint ians, uint iins) {
	unordered_map<uint, void*> arr;
	arr.reserve(181440);

	stack<pair<const uint, void*>*> stk1;
	stack<pair<const uint, void*>*> stk2;

	pair<const uint, void*>* pCur;

	arr[iins] = nullptr;
	auto tmp = arr.find(iins);
	stk1.push(&(*tmp));

	while (!stk1.empty()) {
		pCur = stk1.top();
		stk1.pop();

		if (pCur->first == ians)
			break;

		for (int i = 0; i < 4; ++i) {
			auto t = arr.try_emplace(turn(i, pCur->first), pCur);
			if (t.second)
				stk2.push(&(*t.first));
		}
		if (stk1.empty())
			swap(stk1, stk2);
	}

	stack<uint> trace;
	trace.push(pCur->first);

	while (pCur->second != nullptr) {
		pCur = (pair<const unsigned int, void*>*)pCur->second;
		trace.push(pCur->first);
	}

	return trace;
}

uint convert(string& s) {
	static unordered_map<char, uint> tmp;
	uint m = 0, k = 0, pos = 0;

	if (tmp.empty()) {
		tmp[' '] = k++;
		for (char& i : s)
			if (tmp.try_emplace(i, k).second)
				++k;
	}

	k = 0;
	int len = s.size();

	for (int i = len - 1; i >= 0; --i) {
		m += tmp[s[i]] << k;
		k += 3;
		if (s[i] == ' ')
			pos = i;
	}

	m += (len - pos - 1) << 27;

	return m;
}

int main() {
	string instr = " buzrepus";
	string anstr = "superzub ";

	uint ians = convert(anstr);
	uint iins = convert(instr);

	auto t1 = chrono::system_clock::now();
	stack<uint> stk = func(ians, iins);
	auto t2 = chrono::system_clock::now();

	chrono::duration<double> elapsed = t2 - t1;

	cout << stk.size() << " turns" << endl << endl;
	while (!stk.empty()) {
		print(stk.top());
		stk.pop();
	}

	cout << "elapsed: " << elapsed.count() << " sec." << endl;

	return 0;
}