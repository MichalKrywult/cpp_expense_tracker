#include "transaction_manager.h"
#include <iostream>

TransactionManager::TransactionManager(Storage &storage)
    : storage(storage)
{
    try
    {
        transactions = storage.load();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading transactions: "
                  << e.what() << '\n';
    }
}

void TransactionManager::ensureNotEmpty() const
{
    if (transactions.empty())
    {
        throw std::runtime_error("No transactions.");
    }
}

void TransactionManager::addTransaction(const Transaction &transaction)
{
    transactions.push_back(transaction);

    try
    {
        storage.save(transactions);
    }
    catch (const std::exception &e)
    {
        transactions.pop_back();
        throw;
    }
}

void TransactionManager::editTransaction(int index, const Transaction &transaction)
{
    ensureNotEmpty();

    if (index < 0 || index >= transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    Transaction oldTransaction = transactions[index];
    transactions[index] = transaction;

    try
    {
        storage.save(transactions);
    }
    catch (const std::exception &e)
    {
        transactions[index] = oldTransaction;
        throw;
    }
}

void TransactionManager::removeTransaction(int index)
{
    ensureNotEmpty();

    if (index < 0 || index >= transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    Transaction removedTransaction = transactions[index];
    transactions.erase(transactions.begin() + (index));

    try
    {
        storage.save(transactions);
    }
    catch (const std::exception &e)
    {
        transactions.insert(transactions.begin() + (index), removedTransaction);
        throw;
    }
}

Transaction TransactionManager::getTransaction(int index) const
{
    ensureNotEmpty();

    if (index < 0 || index >= transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }
    Transaction transaction = transactions[index];
    return transaction;
}

std::vector<Transaction> TransactionManager::getAllTransactions() const
{
    ensureNotEmpty();
    return transactions;
}

std::vector<Transaction> TransactionManager::searchTransactionByTitle(
    const std::string &title) const
{
    ensureNotEmpty();

    if (title.empty())
    {
        throw std::invalid_argument("Title cannot be empty.");
    }

    std::vector<Transaction> result;

    for (const auto &transaction : transactions)
    {
        if (transaction.getTitle() == title)
        {
            result.push_back(transaction);
        }
    }

    return result;
}

Summary TransactionManager::calculateSummary() const
{
    ensureNotEmpty();

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
