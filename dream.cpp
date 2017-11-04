#include <algorithm>
#include <ctime>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main() {
	int n = 1e5, k = 42, msum = 0;
	vector<int> vec(n + 1);

	generate(vec.begin() + 1, vec.end(), []() {return rand() % 1000;});
	sort(vec.begin(), vec.end());
	partial_sum(vec.begin(), vec.end(), vec.begin());

	for (int i = 1, j = 3; j <= n;)	{
		while ((j <= n) && (vec[j] - vec[j - 1] <= vec[i + 1] - vec[i - 1] + k))
			++j;

		if (vec[j - 1] - vec[i - 1] > msum)
			msum = vec[j - 1] - vec[i - 1];

		while ((i < j) && (vec[j] - vec[j - 1] > vec[i + 1] - vec[i - 1] + k))
			++i;
	}

	cout << msum << endl;
}
