#include "transaction_manager.h"
#include <iostream>

bool TransactionManager::ensureNotEmpty() const
{
    if (transactions.empty())
    {
        std::cout << "No transactions.\n";
        return false;
    }

    return true;
}

void TransactionManager::addTransaction(const Transaction &transaction)
{
    transactions.push_back(transaction);
}

void TransactionManager::showTransactions() const
{
    if (!ensureNotEmpty())
        return;

    for (int i = 0; i < transactions.size(); i++)
    {
        std::cout << i + 1 << ". ";
        transactions[i].print();
    }
}

void TransactionManager::removeTransaction(int index)
{
    if (index < 1 || index > transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    transactions.erase(transactions.begin() + (index - 1));
}

void TransactionManager::searchTransactionByTitle(const std::string &title) const
{
    if (!ensureNotEmpty())
        return;

    bool found = false;

    for (const auto &transaction : transactions)
    {
        if (transaction.getTitle() == title)
        {
            transaction.print();
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No transactions found.\n";
    }
}