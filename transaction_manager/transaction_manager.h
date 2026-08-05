#pragma once

#include "../transaction/transaction.h"
#include <vector>

struct Summary
{
    double income;
    double expense;
    double balance;
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
};