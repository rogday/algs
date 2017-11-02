#include <iostream>
#include <iomanip>
#include <chrono>
#include <stack>

using namespace std;

int n = 14;
int m = 16;

int grens = 0;

int** arr;
int** printarr;
int*** sarr;

int x0 = 5;
int y0 = 1;
int x1 = n;
int y1 = m;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

struct Node {
	int lvl;
	int x;
	int y;
};

void generate() {
	arr = new int*[n + 2 ];
	printarr = new int*[n + 2 ];
	int tmp;

	for (int i = 0; i <= n + 1; ++i) {
		arr[i] = new int[m + 2];
		printarr[i] = new int[m + 2];
		for (int k = 0; k <= m + 1; ++k) {
			tmp = rand() - RAND_MAX / 2;
			if (i * k == 0 || i == n + 1 || k == m + 1)
				arr[i][k] = n * m + 1;
			else if (tmp < 0)
				arr[i][k] = n * m;
			else
				arr[i][k] = 0;
			printarr[i][k] = arr[i][k];
		}
	}

	arr[x0][y0] = 0;
	printarr[x0][y0] = 0;

	arr[x1][y1] = 0;
	printarr[x1][y1] = 0;

	sarr = new int**[grens + 1];
	for (int i = 0; i < grens + 1; ++i) {
		sarr[i] = new int*[n + 2];
		for (int k = 0; k < n + 2; ++k) {
			sarr[i][k] = new int[m + 2];
			for (int l = 0; l < m + 2; ++l)
				sarr[i][k][l] = arr[k][l];
		}
	}
}

#if defined _WIN32 || defined _WIN64
#include <windows.h>

HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void show(int** ar) {
	cout << '\n';
	for (int i = 0; i <= n + 1 ; ++i) {
		for (int k = 0; k <= m + 1 ; ++k) {
			if ((i == x0 && k == y0) || (i == x1 && k == y1))
				SetConsoleTextAttribute(hConsole, 0x00);
			else if (ar[i][k] >= n * m)
				SetConsoleTextAttribute(hConsole, 0x10);
			else if (ar[i][k] == -1)
				SetConsoleTextAttribute(hConsole, 0x20);
			else if (ar[i][k] == -2)
				SetConsoleTextAttribute(hConsole, 0xE0);
			else
				SetConsoleTextAttribute(hConsole, 0xF0);

			cout << "  ";
		}
		cout  << "\n";
	}
	cout << "\n";
	SetConsoleTextAttribute(hConsole, 0x07);
}
#else
void show(int** ar) {
	cout << '\n';
	for (int i = 0; i <= n + 1 ; ++i) {
		for (int k = 0; k <= m + 1 ; ++k) {
			if ((i == x0 && k == y0) || (i == x1 && k == y1))
				cout <<  "\e[40m  ";
			else if (ar[i][k] >= n * m)
				cout <<  "\e[44m  ";
			else if (ar[i][k] == -1)
				cout <<  "\e[42m  ";
			else if (ar[i][k] == -2)
				cout <<  "\e[103m  ";
			else
				cout << "\e[107m  ";
		}
		cout  << "\e[0m\n";
	}
	cout << "\n\e[0;0m";
}
#endif

void calcpath(int row, int col, int lvl) {
	int cur = sarr[lvl][row][col];

	if (arr[row][col] == n * m)
		printarr[row][col] = -2;
	else
		printarr[row][col] = -1;

	int lx = -1, ly = -1;
	for (int j = 0; j < 4; ++j) {
		int ix = row + dx[j], iy = col + dy[j];

		if (lvl > 0 && sarr[lvl - 1][ix][iy] + 1 == cur) {
			do {
				lvl--;
			} while (lvl > 0 && sarr[lvl - 1][ix][iy] + 1 == cur);

			calcpath(ix, iy, lvl);
			return;
		}

		if (sarr[lvl][ix][iy] + 1 == cur) {
			lx = ix;
			ly = iy;
		}
	}
	if (lx != -1 && ly != -1)
		calcpath(lx, ly, lvl);
}

int path(int x, int y) {
	int s = 1;
	stack<Node> stk_1;
	stack<Node> stk_2;

	for (int i = 0; i < grens + 1; ++i) {
		sarr[i][x][y] = s;
		stk_1.push({i, x, y});
	}

	Node t;
	bool exit = false;
	int olvl = grens + 1;

	while (!stk_1.empty() && !exit) {
		while (!stk_1.empty()) {
			t = stk_1.top();
			stk_1.pop();

			if (t.x == x0 && t.y == y0) {
				if (olvl > t.lvl)
					olvl = t.lvl;
				exit = true;
			}

			for (int k = 0; k < 4; ++k ) {
				int i = t.x + dx[k], j = t.y + dy[k];

				if (sarr[t.lvl][i][j] != n * m + 1) {
					if (sarr[t.lvl][i][j] == 0) {
						sarr[t.lvl][i][j] = s + 1;
						stk_2.push({t.lvl, i, j});
					} else if (sarr[t.lvl][i][j] == n * m && t.lvl != grens && sarr[t.lvl + 1][i][j] == n * m) {
						sarr[t.lvl + 1][i][j] = s + 1;
						stk_2.push({t.lvl + 1, i, j});
					}
				}
			}
		}
		++s;
		stk_1.swap(stk_2);
	}

	return olvl;
}

void magic() {
	auto t1 = chrono::system_clock::now();
	int lvl = path(x1, y1);
	auto t2 = chrono::system_clock::now();

	chrono::duration<double> elapsed = t2 - t1;

	if (lvl != grens + 1) {
		calcpath(x0, y0, lvl);
		show(printarr);
		cout << "We'd found shortest path using " << lvl << " grenades, length is " << sarr[lvl][x0][y0] - 1 << "." << endl << "Elapsed time: " << elapsed.count() << " sec." << endl;
	} else {
		show(arr);
		cout << "It seems like path doesnt exist :c" << endl;
	}
}

int main(int num, char* str[]) {
	srand(time(nullptr));

	if (num == 2)
		grens = atoi(str[1]);
	if (num >= 4) {
		grens = atoi(str[1]);
		n = atoi(str[2]);
		m = atoi(str[3]);
		x1 = n;
		y1 = m;
		if (num == 7) {
			x0 = atoi(str[4]);
			y0 = atoi(str[5]);
			x1 = atoi(str[6]);
			y1 = atoi(str[7]);
		}
	};

	generate();
	magic();
}
