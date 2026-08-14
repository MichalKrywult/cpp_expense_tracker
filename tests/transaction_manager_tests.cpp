#include <gtest/gtest.h>
#include <filesystem>

#include "../transaction_manager/transaction_manager.h"

TEST(TransactionManagerTest, GetTransactionThrowsWhenEmpty)
{
    std::filesystem::remove("transaction_manager_test.csv");

    Storage storage("transaction_manager_test.csv");
    TransactionManager manager(storage);

    EXPECT_THROW(
        manager.getTransaction(1),
        std::runtime_error);

    std::filesystem::remove("transaction_manager_test.csv");
}

TEST(TransactionManagerTest, AddsTransaction)
{
    std::filesystem::remove("transaction_manager_test.csv");

    Storage storage("transaction_manager_test.csv");
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);
    Transaction result = manager.getTransaction(1);

    EXPECT_EQ(result.getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(result.getAmount(), 5000.0);
    EXPECT_EQ(result.getCategory(), "Job");
    EXPECT_EQ(result.getDate(), "2022-01-14");
    EXPECT_EQ(result.getType(), TransactionType::Income);

    std::filesystem::remove("transaction_manager_test.csv");
}

TEST(TransactionManagerTest, ReturnsAddedTransaction)
{
    std::filesystem::remove("transaction_manager_test.csv");

    Storage storage("transaction_manager_test.csv");
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);
    Transaction result = manager.getTransaction(1);

    EXPECT_EQ(result.getTitle(), "Salary");

    std::filesystem::remove("transaction_manager_test.csv");
}

TEST(TransactionManagerTest, GetTransactionThrowsForInvalidIndex)
{
    std::filesystem::remove("transaction_manager_test.csv");
    Storage storage("transaction_manager_test.csv");
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);

    EXPECT_THROW(
        manager.getTransaction(2),
        std::out_of_range);

    std::filesystem::remove("transaction_manager_test.csv");
}