#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "../transaction_manager/transaction_manager.h"

class TransactionManagerTest : public ::testing::Test
{
protected:
    const std::string filename = "transaction_manager_test.csv";

    void SetUp() override
    {
        std::filesystem::remove(filename);
    }

    void TearDown() override
    {
        std::filesystem::remove(filename);
    }
};

TEST_F(TransactionManagerTest, GetTransactionThrowsWhenEmpty)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    EXPECT_THROW(
        manager.getTransaction(0),
        std::runtime_error);
}

TEST_F(TransactionManagerTest, AddsTransaction)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);
    Transaction result = manager.getTransaction(0);

    EXPECT_EQ(result.getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(result.getAmount(), 5000.0);
    EXPECT_EQ(result.getCategory(), "Job");
    EXPECT_EQ(result.getDate(), "2022-01-14");
    EXPECT_EQ(result.getType(), TransactionType::Income);
}

TEST_F(TransactionManagerTest, GetTransactionThrowsForInvalidIndex)
{
    Storage storage(filename);
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
}

TEST_F(TransactionManagerTest, RemovesTransaction)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    for (int i = 0; i < 3; i++)
    {
        Transaction transaction(
            "Salary",
            i,
            "Job",
            "2022-01-14",
            TransactionType::Income);

        manager.addTransaction(transaction);
    }

    manager.removeTransaction(1);

    EXPECT_EQ(manager.getTransaction(0).getAmount(), 0);
    EXPECT_EQ(manager.getTransaction(1).getAmount(), 2);
}

TEST_F(TransactionManagerTest, RemoveThrowsWhenEmpty)
{

    Storage storage(filename);
    TransactionManager manager(storage);

    EXPECT_THROW(manager.removeTransaction(0), std::runtime_error);
}

TEST_F(TransactionManagerTest, RemoveThrowsWhenNegativeIndex)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);
    manager.addTransaction(transaction);

    EXPECT_THROW(manager.removeTransaction(-1), std::out_of_range);
}

TEST_F(TransactionManagerTest, RemoveThrowsWhenIndexOutOfRange)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);
    manager.addTransaction(transaction);

    EXPECT_THROW(manager.removeTransaction(1), std::out_of_range);
}