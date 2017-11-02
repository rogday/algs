#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void count_sort(vector<int>& vec) {
	vector<int> tmp(1001);

	for (auto& i : vec)
		tmp[i]++;

	for (int i = 0, k = 0; i < 1001; ++i) {
		fill_n(vec.begin() + k, tmp[i], i);
		k += tmp[i];
	}
}

int main() {
	istream_iterator<int> inst(cin);
	int n = *(inst++), k = *(inst++);

	vector<int> vec(n);
	copy_n(inst, n, vec.begin());
	count_sort(vec);

	int msum = 0, sum;

	for (int i = 0; i < n - 1; ++i)	{
		sum = vec[i] + vec[i + 1];

		for (int j = i + 2; (j < n) && (vec[j] <= vec[i] + vec[i + 1] + k); ++j)
			sum += vec[j];

		if (sum > msum)
			msum = sum;
	}

	cout << msum << endl;
}