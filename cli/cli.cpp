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

void CLI::printTransaction(const Transaction &transaction)
{
    std::cout << transaction.getTypeString()
              << " | " << transaction.getTitle()
              << " | " << transaction.getAmount()
              << " | " << transaction.getDate()
              << " | " << transaction.getCategory() << "\n";
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
            double value = std::stod(input, &position);

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
    std::string title = readString("Title: ");
    double amount = readDouble("Amount: ");
    std::string date = readString("Date (YYYY-MM-DD): ");

    return Transaction(title, amount, category, date, type);
}

void CLI::showSummary()
{
    try
    {
        Summary summary = manager.calculateSummary();

        std::cout << "========== SUMMARY ==========\n";
        std::cout << "Income : " << summary.income << '\n';
        std::cout << "Expense: " << summary.expense << '\n';
        std::cout << "Balance: " << summary.balance << '\n';
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}

void CLI::showCategoriesSummary()
{
    try
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
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}

void CLI::handleAddingTransaction()
{
    while (true)
    {
        try
        {
            Transaction transaction = readTransaction();
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

void CLI::handleEditingTransaction()
{
    showTransactions();

    int number = readInt("Transaction number to edit: ") - 1;

    while (true)
    {
        try
        {
            Transaction transaction = readTransaction();
            manager.editTransaction(number, transaction);
            std::cout << "Transaction edited successfully.\n";
            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\n";
        }
    }
}

void CLI::showTransactions()
{
    try
    {
        std::vector<Transaction> allTransactions = manager.getAllTransactions();
        for (size_t i = 0; i < allTransactions.size(); i++)
        {
            std::cout << i + 1 << ". ";
            printTransaction(allTransactions[i]);
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "\n";
    }
}

void CLI::searchTransactionByTitle()
{
    std::string title = readString("Title to search: ");
    std::vector<Transaction> result = manager.searchTransactionByTitle(title);

    if (result.empty())
    {
        std::cout << "No transactions found.\n";
    }
    else
    {
        for (size_t i = 0; i < result.size(); i++)
        {
            std::cout << i + 1 << ". ";
            printTransaction(result[i]);
        }
    }
}

void CLI::handleRemovingTransaction()
{
    try
    {
        showTransactions();

        int number = readInt("Transaction number to remove: ") - 1;
        std::string choice;

        do
        {
            std::cout << "Are you sure you want to remove transaction:\n";

            try
            {
                Transaction transaction = manager.getTransaction(number);
                printTransaction(transaction);
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
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
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