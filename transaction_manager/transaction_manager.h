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

public:
    TransactionManager();
    bool addTransaction(const Transaction &transaction);
    void showTransactions() const;
    void searchTransactionByTitle(const std::string &title) const;
    bool ensureNotEmpty() const;
    bool saveTransactionsSafely();
    void removeTransaction(int index);
    void editTransaction(int index, const Transaction &transaction);
    void showOneTransaction(int index) const;
    Summary calculateSummary() const;
    std::map<std::string, Summary> calculateCategoriesSummary() const;
};