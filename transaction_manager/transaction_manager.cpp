#include "transaction_manager.h"
#include <iostream>

TransactionManager::TransactionManager()
    : storage("transactions.csv")
{
    transactions = storage.load();
}

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
    storage.save(transactions);
}

void TransactionManager::showTransactions() const
{
    if (!ensureNotEmpty())
        return;

    for (size_t i = 0; i < transactions.size(); i++)
    {
        std::cout << i + 1 << ". ";
        transactions[i].print();
    }
}

void TransactionManager::removeTransaction(int index)
{
    if (!ensureNotEmpty())
        return;

    if (index < 1 || index > transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    transactions.erase(transactions.begin() + (index - 1));
    storage.save(transactions);
}

Summary TransactionManager::calculateSummary() const
{
    if (!ensureNotEmpty())
        return Summary{0.0, 0.0, 0.0};

    Summary summary{0.0, 0.0, 0.0};

    for (const auto &transaction : transactions)
    {
        if (transaction.getType() == TransactionType::Income)
            summary.income += transaction.getAmount();
        else
            summary.expense += transaction.getAmount();
    }

    summary.balance = summary.income - summary.expense;
    return summary;
}

std::map<std::string, Summary> TransactionManager::calculateCategoriesSummary() const
{
    std::map<std::string, Summary> summary;

    for (const auto &transaction : transactions)
    {
        Summary &categorySummary = summary[transaction.getCategory()];

        if (transaction.getType() == TransactionType::Income)
        {
            categorySummary.income += transaction.getAmount();
        }
        else
        {
            categorySummary.expense += transaction.getAmount();
        }

        categorySummary.balance = categorySummary.income - categorySummary.expense;
    }

    return summary;
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