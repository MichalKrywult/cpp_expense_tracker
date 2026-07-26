#pragma once

#include <string>

enum class TransactionType
{
    Income,
    Expense
};

struct Transaction
{
    std::string title;
    double amount;
    std::string category;
    std::string date;
    TransactionType type;

    Transaction(
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
    }
};