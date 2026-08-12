#pragma once

#include "../transaction/transaction.h"
#include "../storage/storage.h"

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
    Storage storage;
    void ensureNotEmpty() const;

public:
    TransactionManager();

    void addTransaction(const Transaction &transaction);
    void editTransaction(int index, const Transaction &transaction);
    void removeTransaction(int index);

    Transaction getTransaction(int index) const;
    std::vector<Transaction> getAllTransactions() const;
    std::vector<Transaction> searchTransactionByTitle(const std::string &title) const;

    Summary calculateSummary() const;
    std::map<std::string, Summary> calculateCategoriesSummary() const;
};