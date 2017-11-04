#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

int main() {
	int n = 1e5;
	int k = 42;

	vector<int> vec(n + 1);

	for (auto& i : vec)
		i = rand() % 1000;

	vec[0] = 0;

	sort(vec.begin(), vec.end());
	partial_sum(vec.begin(), vec.end(), vec.begin());

	int msum = 0;

	for (int i = 1, j = 3; j <= n;)	{
		while ((j <= n) && (vec[j] - vec[j - 1] <= vec[i + 1] - vec[i - 1] + k))
			++j;

		if (vec[j - 1] - vec[i - 1] > msum)
			msum = vec[j - 1] - vec[i - 1];

		while ((i + 1 < j) && (vec[j] - vec[j - 1] > vec[i + 1] - vec[i - 1] + k))
			++i;
	}

	cout << msum << endl;
}
