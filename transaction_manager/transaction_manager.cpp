#include "transaction_manager.h"
#include <iostream>

TransactionManager::TransactionManager()
    : storage("transactions.csv")
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

void TransactionManager::showTransactions() const
{
    ensureNotEmpty();

    for (size_t i = 0; i < transactions.size(); i++)
    {
        std::cout << i + 1 << ". ";
        transactions[i].print();
    }
}

void TransactionManager::removeTransaction(int index)
{
    ensureNotEmpty();

    if (index < 1 || index > transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    auto removedTransaction = transactions[index - 1];
    transactions.erase(transactions.begin() + (index - 1));

    try
    {
        storage.save(transactions);
    }
    catch (const std::exception &e)
    {
        transactions.insert(transactions.begin() + (index - 1), removedTransaction);
        throw;
    }
}

void TransactionManager::editTransaction(int index, const Transaction &transaction)
{
    ensureNotEmpty();

    if (index < 1 || index > transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    Transaction oldTransaction = transactions[index - 1];
    transactions[index - 1] = transaction;

    try
    {
        storage.save(transactions);
    }
    catch (const std::exception &e)
    {
        transactions[index - 1] = oldTransaction;
        throw;
    }
}

void TransactionManager::showOneTransaction(int index) const
{
    ensureNotEmpty();

    if (index < 1 || index > transactions.size())
    {
        throw std::out_of_range("Invalid transaction number.");
    }

    transactions[index - 1].print();
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

void TransactionManager::searchTransactionByTitle(const std::string &title) const
{
    ensureNotEmpty();

    if (title.empty())
    {
        std::cout << "Please enter a non-empty title.\n";
        return;
    }

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