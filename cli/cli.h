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

    void handleAddingTransaction();
    void showTransactions();
    void searchTransactionByTitle();

public:
    CLI(TransactionManager &manager);

    void run();
};