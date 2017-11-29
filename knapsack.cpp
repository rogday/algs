#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

typedef vector<pair<int, int>> Items;

namespace std {
pair<int, int> operator+(const pair<int, int>& a, const pair<int, int>& b) {
	return make_pair(a.first + b.first, a.second + b.second);
}
}  // namespace std

Items slice(Items& vec, int m) {
	Items v;
	for (int i = 0; i < vec.size(); ++i)
		if (m & (1 << i)) v.push_back(vec[i]);
	return v;
}

int main() {
	int n, w;
	cin >> n >> w;

	Items arr(n);
	pair<int, int> mp(w, 0);

	for (auto& i : arr) cin >> i.first >> i.second;

	for (int i = 1; i < (1 << n); ++i) {
		Items tarr = slice(arr, i);
		auto tmp = accumulate(tarr.begin(), tarr.end(), make_pair(0, 0));
		if (mp.first >= tmp.first && mp.second < tmp.second) mp.second = tmp.second;
	}

	cout << "price: " << mp.second << endl;
}