#include <iostream>

#include "cli.h"
#include "../transaction/transaction.h"

CLI::CLI(TransactionManager &manager)
    : manager(manager)
{
}

std::string CLI::readString(const std::string &prompt)
{
    std::string value;

    std::cout << prompt;
    std::getline(std::cin, value);

    return value;
}

int CLI::readInt(const std::string &prompt)
{
    while (true)
    {
        std::string input = readString(prompt);

        try
        {
            return std::stoi(input);
        }
        catch (...)
        {
            std::cout << "Invalid number. Try again.\n";
        }
    }
}

double CLI::readDouble(const std::string &prompt)
{
    while (true)
    {
        std::string input = readString(prompt);

        try
        {
            return std::stod(input);
        }
        catch (...)
        {
            std::cout << "Invalid number. Try again.\n";
        }
    }
}

TransactionType CLI::readTransactionType()
{
    while (true)
    {
        std::string input = readString("Type (Income, Expense): ");

        if (input == "Income" || input == "income" || input == "i")
            return TransactionType::Income;

        if (input == "Expense" || input == "expense" || input == "e")
            return TransactionType::Expense;

        std::cout << "Invalid type. Try again.\n";
    }
}

void CLI::handleAddingTransaction()
{
    Transaction transaction(
        readString("Title: "),
        readDouble("Amount: "),
        readString("Category: "),
        readString("Date: "),
        readTransactionType());

    manager.addTransaction(transaction);
}

void CLI::run()
{
    while (true)
    {
        std::cout << "1. Add\n";
        std::cout << "2. Show\n";
        std::cout << "0. Exit\n";

        switch (readInt("Choice: "))
        {
        case 1:
            handleAddingTransaction();
            break;
        case 2:
            break;
        case 0:
            std::cout << "Goodbye!";
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}