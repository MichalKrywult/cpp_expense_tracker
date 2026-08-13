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

    void validateDate(const std::string &date);
    void validateTitle(const std::string &title);
    void validateAmount(const double amount);
    void validateType(const TransactionType &type);
    void validateCategory(const std::string &category);

public:
    Transaction(
        std::string title,
        double amount,
        std::string category,
        std::string date,
        TransactionType type);

    std::string getTitle() const;
    double getAmount() const;
    std::string getCategory() const;
    std::string getDate() const;
    TransactionType getType() const;
    std::string getTypeString() const;
};