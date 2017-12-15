#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;

  int arr[16][16], vec[17];

  for (size_t i = 0; i < n; ++i)
    for (size_t k = 0; k < n; ++k)
      cin >> arr[i][k];

  generate(vec, vec + n, [m = 0]() mutable { return m++; });
  vec[n] = 0;

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
  } while (next_permutation(vec + 1, vec + n));

  cout << msum;
}
