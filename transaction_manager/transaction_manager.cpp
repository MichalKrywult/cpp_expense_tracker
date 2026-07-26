#include "transaction_manager.h"
#include <iostream>

void TransactionManager::addTransaction(const Transaction &transaction)
{
    transactions.push_back(transaction);
}

void TransactionManager::showTransactions() const
{

    if (transactions.size() == 0)
    {
        std::cout << "No transactions.\n";
        return;
    }

    for (const auto &transaction : transactions)
    {
        transaction.print();
    }
}