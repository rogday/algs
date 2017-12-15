#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;

  vector<vector<int>> arr(n, vector<int>(n));

  for (auto &i : arr)
    for (auto &k : i)
      cin >> k;

  vector<int> vec(n + 1);
  generate(vec.begin(), vec.end() - 1, [m = 0]() mutable { return m++; });

  int sum, msum = INT32_MAX;

  do {
    sum = 0;
    for (size_t i = 0; i < n; ++i)
      if (sum < msum)
        sum += arr[vec[i]][vec[i + 1]];
      else {
        sum = INT32_MAX;
        break;
      }

    if (msum > sum)
      msum = sum;
  } while (next_permutation(vec.begin() + 1, vec.end() - 1));

  cout << msum;
}
