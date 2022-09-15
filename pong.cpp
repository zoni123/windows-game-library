#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <Windows.h>

bool finished, winner;

bool top, side;

int height = 11, length = 31;
int score_a, score_b, i, j;

int p_pos = height / 2;
int e_pos = height / 2;
int b_x = length / 2, b_y = height / 2;

void scores()
{
	std::string numbers[20];
	numbers[0] = "    *****\n    *   *\n    *   *\n    *   *\n    *****";
	numbers[1] = "        *\n        *\n        *\n        *\n        *";
	numbers[2] = "    *****\n        *\n    *****\n    *\n    *****";
	numbers[3] = "    *****\n        *\n    *****\n        *\n    *****";
	numbers[4] = "    *   *\n    *   *\n    *****\n        *\n        *";
	numbers[5] = "    *****\n    *\n    *****\n        *\n    *****";
	numbers[6] = "    *\n    *\n    *****\n    *   *\n    *****";
	numbers[7] = "    *****\n        *\n        *\n        *\n        *";
	numbers[8] = "    *****\n    *   *\n    *****\n    *   *\n    *****";
	numbers[9] = "    *****\n    *   *\n    *****\n        *\n        *";
	numbers[10] = "             *****\n                      *   *\n                      *   *\n                      *   *\n                      *****";
	numbers[11] = "                 *\n                          *\n                          *\n                          *\n                          *";
	numbers[12] = "             *****\n                          *\n                      *****\n                      *\n                      *****";
	numbers[13] = "             *****\n                          *\n                      *****\n                          *\n                      *****";
	numbers[14] = "             *   *\n                      *   *\n                      *****\n                          *\n                          *";
	numbers[15] = "             *****\n                      *\n                      *****\n                          *\n                      *****";
	numbers[16] = "             *\n                      *\n                      *****\n                      *   *\n                      *****";
	numbers[17] = "             *****\n                          *\n                          *\n                          *\n                          *";
	numbers[18] = "             *****\n                      *   *\n                      *****\n                      *   *\n                      *****";
	numbers[19] = "             *****\n                      *   *\n                      *****\n                          *\n                          *";
	if (i == 2 && j == 3)
		std::cout << numbers[score_a];
	if (i == 2 && j == 3)
		std::cout << numbers[score_b + 10];
	if (score_a == 10)
	{
		finished = 1;
		winner = 1;
	}
	if (score_b == 10)
	{
		finished = 1;
		winner = 0;
	}
}

void ball()
{
	if (i == b_y && j == b_x)
	{
		std::cout << 'O';
		j++;
	}
}

void ball_movement()
{
	if (!top)
	{
		b_y--;
		if (b_y == 1)
			top = 1;
	}
	if (top)
	{
		b_y++;
		if (b_y == height - 2)
			top = 0;
	}
	if (!side)
	{
		b_x--;
		if ((b_x == 2) && (b_y == p_pos || b_y == p_pos - 1 || b_y == p_pos + 1))
			side = 1;
	}
	if (side)
	{
		b_x++;
		if ((b_x == length - 6) && (b_y == e_pos || b_y == e_pos - 1 || b_y == e_pos + 1))
			side = 0;
	}
}

void movement()
{
	if (GetKeyState('W') & 0x8000 && p_pos > 2)
		p_pos--;
	if (GetKeyState('S') & 0x8000 && p_pos < height - 3)
		p_pos++;
	if (GetKeyState(VK_UP) & 0x8000 && e_pos > 2)
		e_pos--;
	if (GetKeyState(VK_DOWN) & 0x8000 && e_pos < height - 3)
		e_pos++;
}

void score_area()
{
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < length; j++)
		{
			if (i == 0)
				std::cout << '-';
			scores();
		}
		std::cout << std::endl;
	}
}

void player()
{
	if ((i == p_pos || i == p_pos - 1 || i == p_pos + 1) && j == 2)
	{
		std::cout << '|';
		j++;
	}
}

void enemy()
{
	if ((i == e_pos || i == e_pos - 1 || i == e_pos + 1) && j == length - 5)
	{
		std::cout << '|';
		j++;
	}
}

void area()
{
	score_area();
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < length; j++)
		{
			if (i == 0 || i == height - 1 || j == 0 || j == length - 1)
				std::cout << '#';
			else
				std::cout << ' ';
			player();
			enemy();
			ball();
		}
		std::cout << std::endl;
	}
}

void point()
{
	if (b_x == 0)
	{
		b_x = length / 2, b_y = height / 2;
		score_b++;
	}
	if (b_x == length - 3)
	{
		b_x = length / 2, b_y = height / 2;
		score_a++;
	}
}

void game()
{
	movement();
	ball_movement();
	point();
	area();
}

int main()
{
	while (!finished)
	{
		game();
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		system("CLS");
	}
	std::cout << "GAME OVER!";
	if (winner)
		std::cout << " - Player 1 Wins!";
	else
		std::cout << " - Player 2 Wins!";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	return 0;
}