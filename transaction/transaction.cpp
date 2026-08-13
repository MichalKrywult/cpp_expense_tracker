#include <stdexcept>
#include <cctype>

#include "transaction.h"

Transaction::Transaction(
    std::string title,
    double amount,
    std::string category,
    std::string date,
    TransactionType type)
    : title(title),
      amount(amount),
      category(category),
      date(date),
      type(type)
{
    // validateTitle(title);
    // validateAmount(amount);
    validateDate(date);
    // validateCategory(category);
    // validateType(type);
}

void Transaction::validateDate(const std::string &date)
{
    if (date.length() != 10)
    {
        throw std::invalid_argument("Wrong format! Use YYYY-MM-DD");
    }

    if (date[4] != '-' || date[7] != '-')
    {
        throw std::invalid_argument("Wrong format! Use YYYY-MM-DD");
    }

    for (size_t i = 0; i < date.length(); i++)
    {
        if (i == 4 || i == 7)
            continue;

        if (!std::isdigit(date[i]))
        {
            throw std::invalid_argument("Wrong format! Use YYYY-MM-DD");
        }
    }

    // int year = std::stoi(date.substr(0, 4)); for future validation (?) if needed
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (month < 1 || month > 12)
    {
        throw std::invalid_argument("Invalid month");
    }

    if (day < 1 || day > 31)
    {
        throw std::invalid_argument("Invalid day");
    }
}

std::string Transaction::getTitle() const
{
    return title;
}

double Transaction::getAmount() const
{
    return amount;
}

std::string Transaction::getCategory() const
{
    return category;
}

std::string Transaction::getDate() const
{
    return date;
}

TransactionType Transaction::getType() const
{
    return type;
}

std::string Transaction::getTypeString() const
{
    switch (type)
    {
    case TransactionType::Income:
        return "Income";
    case TransactionType::Expense:
        return "Expense";
    }

    return "Unknown";
}