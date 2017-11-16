#include <stack>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const int n = 14;
const int m = 16;

typedef array < array < int, m + 2 >, n + 2 > Map;

int grens = 4;

Map arr;
Map printarr;
vector<Map> sarr;

int x0 = 1;
int my_y0 = 1;
int x1 = n;
int my_y1 = m;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

void generate() {
	int tmp;

	for (int i = 0; i <= n + 1; ++i) {
		for (int k = 0; k <= m + 1; ++k) {
			tmp = rand() - RAND_MAX / 2;
			if (i * k == 0 || i == n + 1 || k == m + 1)
				arr[i][k] = n * m + 1;
			else if (tmp < 0)
				arr[i][k] = n * m;
			else
				arr[i][k] = 0;
		}
	}

	arr[x0][my_y0] = arr[x1][my_y1] = 0;
	printarr = arr;

	sarr.resize(grens + 1);
	fill(sarr.begin(), sarr.end(), arr);
}

void flip(int x, int y) {
	if (printarr[x][y] == 0)
		printarr[x][y] = n * m;
	else if (printarr[x][y] != n * m + 1)
		printarr[x][y] = 0;
}

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
	struct Node {
		int lvl;
		int x;
		int y;
	};

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

			if (t.x == x0 && t.y == my_y0) {
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

void show() {
	VideoMode vm = VideoMode::getDesktopMode();

	int size = min(vm.width, vm.height) * 2 / (max(m + 2, n + 2) * 3);

	RenderWindow window(VideoMode(size * (m + 2), size * (n + 2)), "Maze with grenades.");
	window.setPosition(Vector2i(vm.width / 4, vm.height / 4));
	Event event;

	RectangleShape rects[n + 2][m + 2];

	bool restart = true;

	while (window.isOpen())	{

		while (window.pollEvent(event))	{
			switch (event.type) {
			case  Event::Closed:
				window.close();
				break;

			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					if (!restart) {
						printarr = arr;
						restart = true;
					}
					flip(event.mouseButton.y / size, event.mouseButton.x / size);
				} else if (event.mouseButton.button == Mouse::Right && restart) {
					restart = false;

					arr = printarr;
					fill(sarr.begin(), sarr.end(), arr);

					int lvl = path(x1, my_y1);

					if (lvl != grens + 1)
						calcpath(x0, my_y0, lvl);
				}
				break;
			}
		}

		window.clear(Color::Black);

		for (int i = 0; i < n + 2; ++i)
			for (int k = 0; k < m + 2; ++k) {
				rects[i][k] = RectangleShape(Vector2f(size, size));
				rects[i][k].setPosition(size * k , size * i  );

				Color clr;

				if ((i == x0 && k == my_y0) || (i == x1 && k == my_y1))
					clr = Color::Black;
				else if (printarr[i][k] >= n * m)
					clr = Color::Blue;
				else if (printarr[i][k] == -1)
					clr = Color::Green;
				else if (printarr[i][k] == -2)
					clr = Color::Yellow;
				else
					clr = Color::White;
				rects[i][k].setFillColor(clr);
				window.draw(rects[i][k]);
			}

		window.display();
	}
}

int main(int num, char* str[]) {
	srand(time(nullptr));
	generate();
	show();
}