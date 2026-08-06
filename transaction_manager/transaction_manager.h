#pragma once

#include "../transaction/transaction.h"
#include <vector>
#include <map>
#include <string>

struct Summary
{
    double income = 0.0;
    double expense = 0.0;
    double balance = 0.0;
};

class TransactionManager
{
private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(const Transaction &transaction);
    void showTransactions() const;
    void searchTransactionByTitle(const std::string &title) const;
    bool ensureNotEmpty() const;
    void removeTransaction(int index);
    Summary calculateSummary() const;
    std::map<std::string, Summary> calculateCategoriesSummary() const;
};