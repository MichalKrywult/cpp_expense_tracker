#pragma once

#include "../transaction/transaction.h"
#include <vector>

class TransactionManager
{
private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(const Transaction &transaction);
    void showTransactions() const;
    void searchTransactionByTitle(const std::string &title) const;
    bool ensureNotEmpty() const;
};