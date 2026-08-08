#pragma once

#include "../transaction_manager/transaction_manager.h"

class CLI
{
private:
    TransactionManager &manager;

    int readInt(const std::string &prompt);
    std::string readString(const std::string &prompt);
    double readDouble(const std::string &prompt);
    TransactionType readTransactionType();
    std::string readDate();
    Transaction readTransaction();

    void handleAddingTransaction();
    void showTransactions();
    void handleRemovingTransaction();
    void searchTransactionByTitle();
    void showSummary();
    void showCategoriesSummary();
    void handleEditingTransaction();

public:
    CLI(TransactionManager &manager);

    void run();
};