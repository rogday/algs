#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <unordered_map>

using uint = unsigned int;

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
		std::cout << ((num & (0b111 << (i * 3))) >> (i * 3)) << ' ';
		if (i % 3 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

std::stack<uint> solve(uint ians, uint iins) {
	std::unordered_map<uint, uint> arr;
	arr.reserve(362880); // 9!

	std::stack<uint> stk1;
	std::stack<uint> stk2;

	uint pCur;
	std::pair<std::unordered_map<uint, uint>::iterator, bool> tmp;
	const std::array<uint (*)(uint), 4> pFunc = {top, bot, left, right};

	arr[iins] = iins;
	stk1.push(iins);

	do {
		pCur = stk1.top();
		stk1.pop();

		for (auto &func : pFunc) {
			tmp = arr.try_emplace(func(pCur), pCur);
			if (tmp.second)
				stk2.push(tmp.first->first);
		}

		if (stk1.empty())
			swap(stk1, stk2);
	} while (pCur != ians);

	std::stack<uint> trace;
	trace.push(pCur);

	while (pCur != arr[pCur]) {
		pCur = arr[pCur];
		trace.push(pCur);
	}

	return trace;
}

uint convert(std::string &s) {
	static std::unordered_map<char, uint> tmp;
	uint m = 0, k = 0, pos = 0;

	if (tmp.empty()) {
		tmp[' '] = k++;
		for (char &i : s)
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
	std::string instr = " buzrepus";
	std::string anstr = "superzub ";

	uint ians = convert(anstr);
	uint iins = convert(instr);

	auto t1 = std::chrono::system_clock::now();
	std::stack<uint> stk = solve(ians, iins);
	auto t2 = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed = t2 - t1;

	std::cout << stk.size() << " turns" << std::endl << std::endl;
	while (!stk.empty()) {
		print(stk.top());
		stk.pop();
	}

	std::cout << "elapsed: " << elapsed.count() << " sec." << std::endl;

	return 0;
}