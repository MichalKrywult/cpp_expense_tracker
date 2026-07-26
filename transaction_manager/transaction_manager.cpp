#include "transaction_manager.h"
#include <iostream>

void TransactionManager::addTransaction(const Transaction &transaction)
{
    transactions.push_back(transaction);
}

void TransactionManager::showTransactions() const
{
    for (const auto &transaction : transactions)
    {
        std::cout << transaction.getTitle() << " "
                  << transaction.getTypeString() << " "
                  << transaction.getAmount() << " "
                  << transaction.getCategory() << " "
                  << transaction.getDate() << '\n';
    }
}