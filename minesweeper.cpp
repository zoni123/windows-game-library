#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <random>

int x = 1, y = 1;
const int width = 25, height = 25;
char a[height][width];
int prev_y, prev_x;
int board[height][width];
char b[height][width];
int c[height][width];
bool nr;

void movement()
{
	if (GetKeyState(VK_UP) & 0x8000 || GetKeyState('W') & 0x8000 && y > 1)
	{
		prev_y = y;
		y--;
		std::this_thread::sleep_for(std::chrono::milliseconds(66));
	}
	if (GetKeyState(VK_DOWN) & 0x8000 || GetKeyState('S') & 0x8000 && y < height - 2)
	{
		prev_y = y;
		y++;
		std::this_thread::sleep_for(std::chrono::milliseconds(66));
	}
	if (GetKeyState(VK_LEFT) & 0x8000 || GetKeyState('A') & 0x8000 && x > 1)
	{
		prev_x = x;
		x--;
		std::this_thread::sleep_for(std::chrono::milliseconds(66));
	}
	if (GetKeyState(VK_RIGHT) & 0x8000 || GetKeyState('D') & 0x8000 && x < width - 2)
	{
		prev_x = x;
		x++;
		std::this_thread::sleep_for(std::chrono::milliseconds(66));
	}
}

void game()
{
	for (int i = 0; i < 2 * height; i++)
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, height - 2);
		int i1 = dist(mt);
		int i2 = dist(mt);
		board[i1][i2] = -1;
	}
	for (int i = 0; i < height - 2; i++)
	{
		for (int j = 0; j < width - 2; j++)
		{
			if (board[i][j - 1] == -1 && board[i][j] != -1 && j != 0)
				board[i][j]++;
			if (board[i][j + 1] == -1 && board[i][j] != -1 && j != width - 3)
				board[i][j]++;
			if (board[i + 1][j] == -1 && board[i][j] != -1 && i != height - 3)
				board[i][j]++;
			if (board[i - 1][j] == -1 && board[i][j] != -1 && i != 0)
				board[i][j]++;
			if (board[i - 1][j - 1] == -1 && board[i][j] != -1 && i != 0 && j != 0)
				board[i][j]++;
			if (board[i + 1][j + 1] == -1 && board[i][j] != -1 && i != height - 3 && j != width - 3)
				board[i][j]++;
			if (board[i + 1][j - 1] == -1 && board[i][j] != -1 && i != height - 3 && j != 0)
				board[i][j]++;
			if (board[i - 1][j + 1] == -1 && board[i][j] != -1 && i != 0 && j != width - 3)
				board[i][j]++;
		}
	}
}

void area()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			a[i][j] = ' ';
	for (int i = 0; i < height; i++)
	{
		a[i][0] = '#';
		a[i][height - 1] = '#';
		a[0][i] = '#';
		a[height - 1][i] = '#';
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			std::cout << a[i][j] << ' ';
		std::cout << '\n';
	}
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			b[i][j] = ' ';
	for (int i = 0; i < height; i++)
	{
		b[i][0] = '#';
		b[i][height - 1] = '#';
		b[0][i] = '#';
		b[height - 1][i] = '#';
	}
}

void fill(int y, int x)
{
	nr = 0;
	if (y < 1 || y > height - 2 || x < 1 || x > width - 2)
		return;
	if (board[y - 1][x - 1] != -1 && c[y][x] == 0 && nr == 0)
	{
		b[y][x] = board[y - 1][x - 1] + '0';
		c[y][x] = 1;
		a[y][x] = b[y][x];
		if (board[y - 1][x - 1] > 0)
		{
			nr = 1;
			return;
		}
		fill(y - 1, x);
		fill(y + 1, x);
		fill(y, x + 1);
		fill(y, x - 1);
	}
	else
		return;
}

void choice()
{
	if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && board[y - 1][x - 1] != -1)
		fill(y, x);
}

void player()
{
	a[y][x] = 'X';
	if (prev_y != 0)
		a[prev_y][x] = b[prev_y][x];
	if (prev_x != 0)
		a[y][prev_x] = b[y][prev_x];
	if (prev_y != 0 && prev_x != 0)
		a[prev_y][prev_x] = b[prev_y][prev_x];
}

void lose()
{
	if (board[y - 1][x - 1] == -1 && (GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000))
	{
		system("cls");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		for (int i = 0; i < height - 2; i++)
		{
			for (int j = 0; j < width - 2; j++)
				std::cout << board[i][j] << ' ';
			std::cout << '\n';
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
		system("cls");
			std::cout << "Game Over!" << std::endl;
		_exit(0);
	}
}

void test_set()
{
	if (a[y][x] != 'X')
	{
		player();
		system("cls");
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				std::cout << a[i][j] << ' ';
			std::cout << '\n';
		}
	}
	choice();
	lose();
}

void render_board()
{
	for (int i = 0; i < height - 2; i++)
	{
		for (int j = 0; j < width - 2; j++)
			std::cout << board[i][j] << ' ';
		std::cout << std::endl;
	}
	system("cls");
}

int main()
{
	area();
	game();
	while (1)
	{
		movement();
		test_set();
	}
	system("pause");
	return 0;
}
