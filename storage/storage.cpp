#include "storage.h"

#include <sstream>
#include <fstream>

Storage::Storage(const std::string &filename)
    : filename(filename)
{
}

Transaction Storage::parseLine(const std::string &line)
{
    std::stringstream ss(line);

    // the csv file is parsed by the semicolons ;
    std::string title;
    std::getline(ss, title, ';');

    std::string amount_temp;
    std::getline(ss, amount_temp, ';');
    double amount = std::stod(amount_temp); // standard library conversion from string to double type

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
    else if (type == "Expense")
    {
        transactionType = TransactionType::Expense;
    }
    else
    {
        throw std::runtime_error("Invalid transaction type in file");
    }

    Transaction transaction(
        title,
        amount,
        category,
        date,
        transactionType);

    return transaction;
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
        // the csv file is parsed by the semicolons ;
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

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open save file");
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        
        // Transaction parsedLine = parseLine(line);
        // transactions.push_back(parsedLine);
        transactions.push_back(parseLine(line));
    }
    return transactions;
}