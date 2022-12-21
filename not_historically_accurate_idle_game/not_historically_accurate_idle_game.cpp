#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <future>
#include <cmath>
#include <fstream>

unsigned long long int money, prices[10], items_held[10], values[10] = {1, 5, 10, 50, 100, 250, 500, 1000, 10000}, add_money[10], sum;
char input, shop_items[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
bool tapped, switches[10];

std::fstream savefile("savefile.txt");

void AddMoney()
{
	sum = 0;
	for (int i = 0; i < 9; i++)
	{
		add_money[i] = items_held[i] * values[i];
		sum = add_money[i] + sum;
		money = money + add_money[i];
	}
}

void Buy()
{
	for (int i = 0; i < 9; i++)
	{
		if ((GetAsyncKeyState(shop_items[i]) & 0x8000) && !switches[i] && money >= prices[i])
		{
			switches[i] = 1;
			items_held[i]++;
			money = money - prices[i];
			prices[i] = (prices[i] + log(prices[i])) * 6 / 5;
		}
		if (GetAsyncKeyState(shop_items[i]) == 0)
			switches[i] = 0;
	}
}

void Shop()
{
	std::cout << "1. Slave - COST: " << prices[0] << " /// OWNED: " << items_held[0] << '\n';
	std::cout << "2. Serf - COST: " << prices[1] << " /// OWNED: " << items_held[1] << '\n';
	std::cout << "3. Free Peasant - COST: " << prices[2] << " /// OWNED: " << items_held[2] << '\n';
	std::cout << "4. Gentleman - COST: " << prices[3] << " /// OWNED: " << items_held[3] << '\n';
	std::cout << "5. Knight - COST: " << prices[4] << " /// OWNED: " << items_held[4] << '\n';
	std::cout << "6. Jester - COST: " << prices[5] << " /// OWNED: " << items_held[5] << '\n';
	std::cout << "7. Lord - COST: " << prices[6] << " /// OWNED: " << items_held[6] << '\n';
	std::cout << "8. King - COST: " << prices[7] << " /// OWNED: " << items_held[7] << '\n';
	std::cout << "9. Emperor - COST: " << prices[8] << " /// OWNED: " << items_held[8] << "\n\n";
}

void Tap()
{
	Buy();
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !tapped)
	{
		tapped = 1;
		money++;
	}
	if (GetAsyncKeyState(VK_SPACE) == 0)
		tapped = 0;
}

void ShowMoney()
{
	while(1)
	{
		Shop();
		std::cout << "CURRENT BALANCE: " << money << '\n' << "GROWTH RATE: " << sum << '\n';
		AddMoney();
		std::this_thread::sleep_for(std::chrono::milliseconds(133));
		system("CLS");
	}
}

void Game()
{
	Tap();
}

void WriteToFile()
{
	savefile.clear();
	savefile.seekg(0);
	for (int i = 0; i < 9; i++)
		savefile << prices[i] << ' ' << items_held[i] << '\n';
	savefile << money;
	savefile.close();
}

void ExitGame()
{
	WriteToFile();
	system("cls");
	std::cout << "EXITING...";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	exit(0);
}

void menu()
{
	int option;
	std::cout << "|--------------|\n| 1. NEW GAME  |\n|              |\n| 2. LOAD GAME |\n|              |\n| 3.  EXIT     |\n|--------------|\n";
	std::cin >> option;
	if (option == 1)
		return;
	if (option == 2)
	{
		for (int i = 0; i < 9; i++)
			savefile >> prices[i] >> items_held[i];
		savefile >> money;
	}
	if (option == 3)
		ExitGame();
}

void SetPrices()
{
	int p = 10;
	for (int i = 0; i < 9; i++)
	{
		prices[i] = p;
		p = p * 10;
	}
}

int main()
{
	SetPrices();
	menu();
	std::cout << "TAP WITH SPACEBAR";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("CLS");
	std::future<void> ShowMoneyAsync = std::async(ShowMoney);
	execution:
	while (1)
	{
		Game();
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			ExitGame();
	}
	return 0;
}
