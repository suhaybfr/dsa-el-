#include "Game.hpp"
#include <iostream>

int main()
{
    int choice = 0;

    std::cout << "===== DSA ENGINE DEMO =====\n";
    std::cout << "1. Survival Game\n";
    std::cout << "2. Platformer Game\n";
    std::cout << "Choose game: ";
    std::cin >> choice;

    if (choice == 1)
    {
        Game game;
        game.run();
    }
    else if (choice == 2)
    {
        runGame2();
    }
    else
    {
        std::cout << "Invalid choice\n";
    }

    return 0;
}
