#include <iostream>
#include <stdexcept>
#include <cctype>
#include <map>

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

std::string CLI::readDate()
{
    std::string value;

    while (true)
    {
        value = readString("Date (YYYY-MM-DD): ");

        if (value.length() != 10)
        {
            std::cout << "Wrong format!\n";
            continue;
        }

        if (value[4] != '-' || value[7] != '-')
        {
            std::cout << "Wrong format!\n";
            continue;
        }

        bool correct = true;

        int year = std::stoi(value.substr(0, 4));
        int month = std::stoi(value.substr(5, 2));
        int day = std::stoi(value.substr(8, 2));

        if (month < 1 || month > 12)
        {
            std::cout << "Invalid month.\n";
            continue;
        }

        if (day < 1 || day > 31)
        {
            std::cout << "Invalid day.\n";
            continue;
        }

        for (size_t i = 0; i < value.length(); i++)
        {
            if (i == 4 || i == 7)
                continue;

            if (!std::isdigit(value[i]))
            {
                correct = false;
                break;
            }
        }

        if (correct)
            return value;

        std::cout << "Wrong format! Use YYYY-MM-DD.\n";
    }
}

int CLI::readInt(const std::string &prompt)
{
    while (true)
    {
        std::string input = readString(prompt);

        try
        {
            // position will store the position where std::stoi stopped reading the number.
            size_t position;

            // Try to convert the string into an integer.
            int value = std::stoi(input, &position);

            // Check if vaules match
            if (position != input.length())
            {
                throw std::invalid_argument("Extra characters");
            }

            return value;
        }
        catch (const std::exception &)
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
            size_t position;
            int value = std::stod(input, &position);

            if (position != input.length())
            {
                throw std::invalid_argument("Extra characters");
            }

            return value;
        }
        catch (const std::exception &)
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

Transaction CLI::readTransaction()
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

    std::string date = readDate();

    return Transaction(title, amount, category, date, type);
}

void CLI::showSummary()
{
    Summary summary = manager.calculateSummary();

    std::cout << "========== SUMMARY ==========\n";
    std::cout << "Income : " << summary.income << '\n';
    std::cout << "Expense: " << summary.expense << '\n';
    std::cout << "Balance: " << summary.balance << '\n';
}

void CLI::showCategoriesSummary()
{
    std::map<std::string, Summary> summaries = manager.calculateCategoriesSummary();

    std::cout << "========== SUMMARY ==========\n";
    for (const auto &[category, summary] : summaries)
    {
        std::cout << category << '\n';
        std::cout << "Income : " << summary.income << '\n';
        std::cout << "Expense: " << summary.expense << '\n';
        std::cout << "Balance: " << summary.balance << "\n\n";
    }
}

void CLI::handleAddingTransaction()
{
    while (true)
    {
        try
        {
            Transaction transaction = readTransaction();

            bool completed = manager.addTransaction(transaction);
            if (completed)
            {
                std::cout << "Transaction added successfully.\n";
                return;
            }
            else
            {
                std::cout << "Something went wrong, transaction wasn't added.\n";
                return;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\nPlease try again.\n\n";
        }
    }
}

void CLI::handleEditingTransaction()
{
    showTransactions();

    int number = readInt("Transaction number to edit: ");

    try
    {
        Transaction transaction = readTransaction();

        manager.editTransaction(number, transaction);

        std::cout << "Transaction edited successfully.\n";
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "\n";
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

void CLI::handleRemovingTransaction()
{
    showTransactions();

    int number = readInt("Transaction number to remove: ");
    std::string choice;

    do
    {
        std::cout << "Are you sure you want to remove transaction:\n";

        try
        {
            manager.showOneTransaction(number);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << '\n';
            return;
        }

        choice = readString("(y/n): ");

        if (choice != "y" && choice != "Y" &&
            choice != "n" && choice != "N")
        {
            std::cout << "Invalid input. Please enter y or n.\n";
        }

    } while (choice != "y" && choice != "Y" && choice != "n" && choice != "N");

    if (choice == "y" || choice == "Y")
    {
        try
        {
            manager.removeTransaction(number);
            std::cout << "Transaction removed successfully.\n";
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << '\n';
        }
    }
    else
    {
        std::cout << "Transaction wasn't removed.\n";
    }
}

void CLI::run()
{
    while (true)
    {
        std::cout << "=====MENU=====\n";
        std::cout << "1. Add\n";
        std::cout << "2. Show\n";
        std::cout << "3. Search by title\n";
        std::cout << "4. Remove\n";
        std::cout << "5. Edit transaction\n";
        std::cout << "6. Summary\n";
        std::cout << "7. Summary for each category\n";
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
        case 4:
            handleRemovingTransaction();
            break;
        case 5:
            handleEditingTransaction();
            break;
        case 6:
            showSummary();
            break;
        case 7:
            showCategoriesSummary();
            break;
        case 0:
            std::cout << "Goodbye!";
            return;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}