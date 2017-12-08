#include <iostream>

int main() {
  int warr[31], parr[31];
  int n, w, mp = 0, tw, tp;
  std::cin >> n >> w;

  for (int i = 0; i < n; ++i)
    std::cin >> warr[i] >> parr[i];

  for (int i = 1; i < (1 << n); ++i) {
    tw = tp = 0;
    for (int k = 0; k < n; ++k)
      if (i & (1 << k)) {
        tw += warr[k];
        tp += parr[k];
      };
    if (w >= tw && mp < tp)
      mp = tp;
  }

  std::cout << "\nPrice: " << mp << '\n';
}
