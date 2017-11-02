#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <unordered_map>

using namespace std;
typedef unsigned int uint;

inline uint top(uint num) {
	int pos = num >> 27;

	if (pos > 5)
		return num;

	pos += 3;
	uint digit = (num & (0b111 << (pos * 3))) >> 9;
	num &= ~(0b111 << (pos * 3));

	num |= digit;
	num += 3 << 27;

	return num;
}

inline uint bot(uint num) {
	int pos = num >> 27;

	if (pos < 3)
		return num;

	pos -= 3;
	uint digit = (num & (0b111 << (pos * 3))) << 9;
	num &= ~(0b111 << (pos * 3));

	num |= digit;
	num -= 3 << 27;

	return num;
}

inline uint left(uint num) {
	int pos = num >> 27;

	if (pos % 3 == 2)
		return num;

	++pos;
	uint digit = (num & (0b111 << (pos * 3))) >> 3;
	num &= ~(0b111 << (pos * 3));

	num |= digit;
	num += 1 << 27;

	return num;
}

inline uint right(uint num) {
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
	pair<unordered_map<uint, void*>::iterator, bool> tmp;

	arr[iins] = nullptr;

	auto tmp2 = arr.find(iins);
	stk1.push(&(*tmp2));

	const array<uint(*)(uint), 4> pFunc = {top, bot, left, right};

	do {
		pCur = stk1.top();
		stk1.pop();

		for (auto& func : pFunc) {
			tmp = arr.try_emplace(func(pCur->first), pCur);
			if (tmp.second)
				stk2.push(&(*tmp.first));
		}

		if (stk1.empty())
			swap(stk1, stk2);
	} while (pCur->first != ians);

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
