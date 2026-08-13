#include <iostream>
#include <stdexcept>

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
    if (title.empty())
    {
        throw std::invalid_argument("Title cannot be empty");
    }

    if (amount <= 0)
    {
        throw std::invalid_argument("Amount must be positive");
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