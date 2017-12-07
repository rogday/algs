#include <iostream>
#include <chrono>

using namespace std;

int main() {
	int warr[31], parr[31];
	int n, w, mp = 0, tw, tp;
	cin >> n >> w;

	for (int i = 0; i < n; ++i) cin >> warr[i] >> parr[i];

	auto start = std::chrono::system_clock::now();

	for (int i = 1; i < (1 << n); ++i) {
		tw = tp = 0;
		for (int k = 0; k < n; ++k)
			if (i & (1 << k)){
				tw += warr[k];
				tp += parr[k];
			};
		if (w >= tw && mp < tp) mp = tp;
	}

  	auto end = std::chrono::system_clock::now();

  	std::chrono::duration<double> elapsed_seconds = end-start;

	cout << "\nPrice: " << mp << "\nTime elapsed: " << elapsed_seconds.count() << " sec.\n";
}
