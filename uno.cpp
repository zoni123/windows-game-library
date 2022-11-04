#include <iostream>
#include <chrono>
#include <thread>
#include <stack>
#include <stdlib.h>
#include <random>
#include <cctype>

int p1_nrc, p2_nrc, chosen_card;
char player1[33], player2[33], colour, cards[109][2], p1_cards[109][2], p2_cards[109][2], played_card[2];
bool turn, firstMove;
std::stack<char> card_stack, col_stack, changing_card_stack, changing_col_stack;

void Win()
{
    if (p1_nrc == 0)
    {
        system("CLS");
        std::cout << player1 << " wins!";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        exit(0);
    }
    if (p2_nrc == 0)
    {
        system("CLS");
        std::cout << player2 << " wins!";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        exit(0);
    }
}

void Cards()
{
    int p, j = 0;
    char col[5] = { 'R', 'G', 'B', 'Y' };

    // 0-9 CARDS
    for (int i = 0; i < 76; i++)
    {
        if (i % 19 == 0)
        {
            j++;
            cards[i][0] = '0';
            p = 1;
        }
        else
        {
            cards[i][0] = p + '0';
            if (i % 2 == 0 && j % 2 == 1)
                p++;
            if (i % 2 == 1 && j % 2 == 0)
                p++;
        }
        cards[i][1] = col[j - 1];
    }

    //ACTION CARDS
    j = 0;
    for (int i = 76; i < 84; i++)
    {
        if (i % 2 == 0)
            j++;
        cards[i][0] = '+';
        cards[i][1] = col[j - 1];
    }
    j = 0;
    for (int i = 84; i < 92; i++)
    {
        if (i % 2 == 0)
            j++;
        cards[i][0] = '>';
        cards[i][1] = col[j - 1];
    }
    j = 0;
    for (int i = 92; i < 100; i++)
    {
        if (i % 2 == 0)
            j++;
        cards[i][0] = '-';
        cards[i][1] = col[j - 1];
    }
    j = 0;

    //WILD CARDS
    for (int i = 100; i < 108; i++)
    {
        if (i < 104)
            cards[i][0] = 'C';
        else
            cards[i][0] = '*';
    }
}

void Shuffle()
{
    for (int i = 0; i < 108; i++)
    {
        static std::random_device rd;
        static std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 107);
        std::swap(cards[i], cards[dist(mt)]);
    }
}

void Distribute()
{
    for (int i = 0; i < 108; i++)
    {
        card_stack.push(cards[i][0]);
        col_stack.push(cards[i][1]);
    }
    for (int i = 0; i < 7; i++)
    {
        p1_cards[i][0] = card_stack.top();
        p1_cards[i][1] = col_stack.top();
        card_stack.pop();
        col_stack.pop();
    }
    for (int i = 0; i < 7; i++)
    {
        p2_cards[i][0] = card_stack.top();
        p2_cards[i][1] = col_stack.top();
        card_stack.pop();
        col_stack.pop();
    }
    p1_nrc = 7;
    p2_nrc = 7;
}

void RemoveCardP1()
{
    changing_card_stack.push(played_card[0]);
    changing_col_stack.push(played_card[1]);
    for (int i = chosen_card - 1; i < p1_nrc; i++)
    {
        p1_cards[i][0] = p1_cards[i + 1][0];
        p1_cards[i][1] = p1_cards[i + 1][1];
    }
    p1_nrc--;
}

void RemoveCardP2()
{
    changing_card_stack.push(played_card[0]);
    changing_col_stack.push(played_card[1]);
    for (int i = chosen_card - 1; i < p2_nrc; i++)
    {
        p2_cards[i][0] = p2_cards[i + 1][0];
        p2_cards[i][1] = p2_cards[i + 1][1];
    }
    p2_nrc--;
}

void DisplayPlayers()
{
    std::cout << "ENTER 0 TO DRAW CARD" << std::endl << std::endl;
    if(turn == 0)
    {
        std::cout << player1 << ": ";
        for (int i = 0; i < p1_nrc; i++)
            std::cout << p1_cards[i][0] << p1_cards[i][1] << ' ';
    }
    std::cout << "/// P1 CARD COUNT: " << p1_nrc;
    std::cout << std::endl;
    if(turn == 1)
    {
        std::cout << player2 << ": ";
        for (int i = 0; i < p2_nrc; i++)
            std::cout << p2_cards[i][0] << p2_cards[i][1] << ' ';
    }
    std::cout << "/// P2 CARD COUNT: " << p2_nrc;
    std::cout << std::endl << std::endl;
}

bool IsValid()
{
    if (chosen_card == 0)
        return 1;
    if ((chosen_card > p1_nrc && turn == 0) || (chosen_card > p2_nrc && turn == 1))
        return 0;
    if(turn == 0)
    {
        if (played_card[0] == p1_cards[chosen_card - 1][0] || played_card[1] == p1_cards[chosen_card - 1][1] || p1_cards[chosen_card - 1][0] == 'C' || p1_cards[chosen_card - 1][0] == '*')
            return 1;
        else
            return 0;
    }
    if (turn == 1)
    {
        if (played_card[0] == p2_cards[chosen_card - 1][0] || played_card[1] == p2_cards[chosen_card - 1][1] || p2_cards[chosen_card - 1][0] == 'C' || p2_cards[chosen_card - 1][0] == '*')
            return 1;
        else
            return 0;
    }
}

void ChangeColour()
{
    if (played_card[0] == 'C' || played_card[0] == '*')
    {
        std::cout << "\nCHANGE COLOUR INTO (R, G, B, Y): ";
        std::cin >> colour;
        colour = toupper(colour);
        if (colour == 'R' || colour == 'G' || colour == 'B' || colour == 'Y')
            played_card[1] = colour;
        else
            ChangeColour();
        std::cout << "\nCOLOUR CHANGED TO: " << colour;
    }
}

void DrawCard()
{
    if (turn == 0)
    {
        p1_cards[p1_nrc][0] = card_stack.top();
        p1_cards[p1_nrc][1] = col_stack.top();
        p1_nrc++;
        card_stack.pop();
        col_stack.pop();
    }
    else
    {
        p2_cards[p2_nrc][0] = card_stack.top();
        p2_cards[p2_nrc][1] = col_stack.top();
        p2_nrc++;
        card_stack.pop();
        col_stack.pop();
    }
}

void Draw2()
{
    DrawCard();
    DrawCard();
}

void Draw4()
{
    Draw2();
    Draw2();
}

void Check()
{
    if (card_stack.empty() && col_stack.empty())
    {
        while (!changing_card_stack.empty())
        {
            card_stack.push(changing_card_stack.top());
            col_stack.push(changing_col_stack.top());
            changing_card_stack.pop();
            changing_col_stack.pop();
        }
    }
}

void Turn()
{
    Check();
    Win();
    if(!firstMove)
    {
        played_card[0] = card_stack.top();
        played_card[1] = col_stack.top();
        card_stack.pop();
        col_stack.pop();
        std::cout << "FIRST CARD: " << played_card[0] << played_card[1] << std::endl;
    }
    std::cout << "Choose a card to play (by writing its index in the deck)" << std::endl;
    std::cin >> chosen_card;
    while (!IsValid())
    {
        std::cout << "Invalid move" << std::endl;
        std::cin >> chosen_card;
    }
    if(chosen_card == 0)
        DrawCard();
    if (chosen_card != 0)
    {
        system("CLS");
        firstMove = 1;
    }
    if (turn == 0)
    {
        if (chosen_card != 0)
        {
            std::cout << player1 << " played " << p1_cards[chosen_card - 1][0] << p1_cards[chosen_card - 1][1];
            played_card[0] = p1_cards[chosen_card - 1][0];
            played_card[1] = p1_cards[chosen_card - 1][1];
        }
        if (p1_cards[chosen_card - 1][0] == '+')
        {
            turn = 1;
            Draw2();
            turn = 0;
        }
        if (p1_cards[chosen_card - 1][0] == '*')
        {
            turn = 1;
            Draw4();
            turn = 0;
        }
        if (p1_cards[chosen_card - 1][0] == '-')
            turn = 0;
        else
            turn = 1;
        if (chosen_card != 0)
            RemoveCardP1();
    }
    else if (turn == 1)
    {
        if (chosen_card != 0)
        {
            std::cout << player2 << " played " << p2_cards[chosen_card - 1][0] << p2_cards[chosen_card - 1][1];
            played_card[0] = p2_cards[chosen_card - 1][0];
            played_card[1] = p2_cards[chosen_card - 1][1];
        }
        if (p2_cards[chosen_card - 1][0] == '+')
        {
            turn = 0;
            Draw2();
            turn = 1;
        }
        if (p2_cards[chosen_card - 1][0] == '*')
        {
            turn = 0;
            Draw4();
            turn = 1;
        }
        if (p2_cards[chosen_card - 1][0] == '-')
            turn = 1;
        else
            turn = 0;
        if (chosen_card != 0)
            RemoveCardP2();
    }
    if (chosen_card != 0)
        ChangeColour();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << std::endl;
}

void Greeting()
{
    char skip;
    std::cout << "WELCOME TO COMMAND-LINE UNO! (2 PLAYERS)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Would you like to skip the tutorial? (y/n)";
    std::cin >> skip;
    std::cin.ignore();
    if (tolower(skip) == 'y')
    {
        system("CLS");
        return;
    }
    system("CLS");
    std::cout << "Here are the rules: " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "1. Each player gets to play their turn. - When it's a player's turn, only his cards are displayed" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "2. It's just like any other game of UNO (hopefully not too buggy)" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Here is a legend explaining the notations of certain cards, take some time to read it" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "R Red\nG Green\nB Blue\nY Yellow\n+ +2\n> Reverse\n- Block\n* +4\nC Change Colour" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    system("CLS");
}

void Game()
{
    while(1)
    {
        DisplayPlayers();
        Turn();
    }
}

int main()
{
    Greeting();
    std::cout << "PLAYER 1: ";
    std::cin.getline(player1, 33);
    std::cout << "PLAYER 2: ";
    std::cin.getline(player2, 33);
    system("CLS");
    Cards();
    Shuffle();
    Distribute();
    changing_card_stack.push(card_stack.top());
    changing_col_stack.push(col_stack.top());
    Game();
    return 0;
}