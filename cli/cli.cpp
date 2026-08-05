#include <iostream>
#include <stdexcept>

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
    while (true)
    {
        try
        {
            TransactionType type = readTransactionType();
            std::string category = readString("Category: ");

            std::string title;
            while (true)
            {
                title = readString("Title: ");

                if (!title.empty())
                    break;

                std::cout << "Title cannot be empty.\n";
            }

            double amount;
            while (true)
            {
                amount = readDouble("Amount: ");

                if (amount > 0)
                    break;

                std::cout << "Amount must be positive.\n";
            }

            std::string date = readString("Date: ");

            Transaction transaction(title, amount, category, date, type);
            manager.addTransaction(transaction);

            std::cout << "Transaction added successfully.\n";
            return;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\nPlease try again.\n\n";
        }
    }
}

void CLI::showTransactions()
{
    manager.showTransactions();
}

void CLI::searchTransactionByTitle()
{
    std::string title = readString("Title to search: ");
    manager.searchTransactionByTitle(title);
}

void CLI::run()
{
    while (true)
    {
        std::cout << "=====MENU=====\n";
        std::cout << "1. Add\n";
        std::cout << "2. Show\n";
        std::cout << "3. Search by title\n";
        std::cout << "0. Exit\n";

        switch (readInt("Choice: "))
        {
        case 1:
            handleAddingTransaction();
            break;
        case 2:
            showTransactions();
            break;
        case 3:
            searchTransactionByTitle();
            break;
        case 0:
            std::cout << "Goodbye!";
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}