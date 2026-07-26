#include "cli.h"
#include <iostream>

CLI::CLI(TransactionManager &manager)
    : manager(manager)
{
}

void CLI::run()
{
    int choice;

    while (true)
    {
        std::cout << "1. Add\n";
        std::cout << "2. Show\n";
        std::cout << "0. Exit\n";

        std::cin >> choice;

        if (choice == 0)
            break;
    }
}