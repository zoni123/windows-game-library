#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <chrono>
#include <thread>

int width, height;

int p_x, p_y;
int i_x = 0, i_y = 0;

bool u, d, l, r;

bool f = 0;
int ln = 1;

void movement()
{		
	if (GetKeyState('S') & 0x8000 && u == 0)
	{
		d = 1;
		u = 0;
		l = 0;
		r = 0;
	}
	if (GetKeyState('W') & 0x8000 && d == 0)
	{
		d = 0;
		u = 1;
		l = 0;
		r = 0;
	}
	if (GetKeyState('A') & 0x8000 && r == 0)
	{
		d = 0;
		u = 0;
		l = 1;
		r = 0;
	}
	if (GetKeyState('D') & 0x8000 && l == 0)
	{
		d = 0;
		u = 0;
		l = 0;
		r = 1;
	}
	if (d)
		p_y++;
	if (u)
		p_y--;
	if (l)
		p_x--;
	if (r)
		p_x++;
	if (p_y > height-2)
		p_y = 1;
	else if (p_x > width-2)
		p_x = 1;
	else if (p_y < 1)
		p_y = height-2;
	else if (p_x < 1)
		p_x = width-2;
}

void growth()
{
	if (i_x == p_x && i_y == p_y)
	{
		f = 0;
		ln++;
	}
}

void fruit()
{
	if (f == 0)
	{
		i_x = rand() % (width - 2) + 1;
		i_y = rand() % (height - 2) + 1;
		f = 1;
	}
	growth();
}

void game_area()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1)
				std::cout << '#';
			else
			{
				if (j == 0 || j == width - 1)
					std::cout << "#";
				else if (i == p_y && j == p_x)
					std::cout << 'O';
				else if (i == i_y && j == i_x)
					std::cout << '@';
				else
					std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Fruits: " << ln-1;
}

void define_area()
{
	std::cout << "ENTER AREA SIZE (GREATER THAN 2X2)\nWIDTH: ";
	std::cin >> width;
	std::cout << "HEIGHT: ";
	std::cin >> height;
	if (width <= 2 && height <= 2)
	{
		system("CLS");
		std::cout << "---INVALID WIDTH AND HEIGHT---\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		system("CLS");
		define_area();
	}
	p_x = width / 2, p_y = height / 2;
}

int main()
{
	define_area();
	while (1)
	{
		fruit();
		movement();
		game_area();
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		system("CLS");
	}
	system("pause");
	return 0;
}