#include "transaction.h"
#include <stdexcept>

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