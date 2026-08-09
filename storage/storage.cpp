#include "storage.h"

#include <sstream>
#include <fstream>

Storage::Storage(const std::string &filename)
    : filename(filename)
{
}

void Storage::save(const std::vector<Transaction> &transactions)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open save file");
    }

    for (const auto &transaction : transactions)
    {
        file << transaction.getTitle()
             << ";"
             << transaction.getAmount()
             << ";"
             << transaction.getCategory()
             << ";"
             << transaction.getDate()
             << ";"
             << transaction.getTypeString()
             << "\n";
    }
}

std::vector<Transaction> Storage::load()
{
    std::vector<Transaction> transactions;
    std::ifstream file(filename);

    if (!file)
    {
        return transactions;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string title;
        std::getline(ss, title, ';');

        std::string amount_temp;
        std::getline(ss, amount_temp, ';');
        double amount = std::stod(amount_temp);

        std::string category;
        std::getline(ss, category, ';');

        std::string date;
        std::getline(ss, date, ';');

        std::string type;
        std::getline(ss, type, ';');
        TransactionType transactionType;
        if (type == "Income")
        {
            transactionType = TransactionType::Income;
        }
        else
        {
            transactionType = TransactionType::Expense;
        }

        Transaction transaction(
            title,
            amount,
            category,
            date,
            transactionType);

        transactions.push_back(transaction);
    }

    return transactions;
}