#pragma once

#include <string>

enum class TransactionType
{
    Income,
    Expense
};

class Transaction
{
private:
    std::string title;
    double amount;
    std::string category;
    std::string date;
    TransactionType type;

public:
    Transaction(
        std::string title,
        double amount,
        std::string category,
        std::string date,
        TransactionType type);

    std::string getTitle() const;
    double getAmount() const;
    std::string getCategory() const; // TODO
    std::string getDate() const;     // TODO
    TransactionType getType() const; // TODO
};