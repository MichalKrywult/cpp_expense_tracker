#include <gtest/gtest.h>

#include "../transaction_manager/transaction_manager.h"

TEST(TransactionManagerTest, GetTransactionThrowsWhenEmpty)
{
    TransactionManager manager;

    EXPECT_THROW(
        manager.getTransaction(1),
        std::runtime_error);
}

TEST(TransactionManagerTest, AddsTransaction)
{
    TransactionManager manager;

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
}

TEST(TransactionManagerTest, ReturnsAddedTransaction)
{
    TransactionManager manager;

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);

    Transaction result = manager.getTransaction(1);

    EXPECT_EQ(result.getTitle(), "Salary");
}

TEST(TransactionManagerTest, GetTransactionThrowsForInvalidIndex)
{
    TransactionManager manager;

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
}