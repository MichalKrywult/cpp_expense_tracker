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

    EXPECT_DOUBLE_EQ(manager.getTransaction(0).getAmount(), 0);
    EXPECT_DOUBLE_EQ(manager.getTransaction(1).getAmount(), 2);
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

TEST_F(TransactionManagerTest, EditsTransaction)
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

    Transaction newTransaction(
        "Rent",
        200.0,
        "House",
        "2022-01-12",
        TransactionType::Expense);

    manager.editTransaction(0, newTransaction);

    EXPECT_EQ(manager.getTransaction(0).getTitle(), "Rent");
    EXPECT_DOUBLE_EQ(manager.getTransaction(0).getAmount(), 200.0);
    EXPECT_EQ(manager.getTransaction(0).getCategory(), "House");
    EXPECT_EQ(manager.getTransaction(0).getDate(), "2022-01-12");
    EXPECT_EQ(manager.getTransaction(0).getType(), TransactionType::Expense);
}

TEST_F(TransactionManagerTest, EditThrowsWhenEmptyTransaction)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_THROW(
        manager.editTransaction(0, transaction),
        std::runtime_error);
}

TEST_F(TransactionManagerTest, EditionThrowsWhenNegativeIndex)
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

    Transaction newTransaction(
        "Rent",
        200.0,
        "House",
        "2022-01-12",
        TransactionType::Expense);

    EXPECT_THROW(manager.editTransaction(-1, newTransaction), std::out_of_range);
}

TEST_F(TransactionManagerTest, EditionThrowsWhenIndexOutOfRange)
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

    Transaction newTransaction(
        "Rent",
        200.0,
        "House",
        "2022-01-12",
        TransactionType::Expense);

    EXPECT_THROW(manager.editTransaction(1, newTransaction), std::out_of_range);
}

TEST_F(TransactionManagerTest, EditDoesNotChangeOtherTransactions)
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
    Transaction newTransaction(
        "Rent",
        3.0,
        "House",
        "2022-01-12",
        TransactionType::Expense);

    manager.editTransaction(1, newTransaction);

    EXPECT_DOUBLE_EQ(manager.getTransaction(0).getAmount(), 0);
    EXPECT_DOUBLE_EQ(manager.getTransaction(1).getAmount(), 3);
    EXPECT_DOUBLE_EQ(manager.getTransaction(2).getAmount(), 2);
}

TEST_F(TransactionManagerTest, SearchByTitleOne)
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

    auto result = manager.searchTransactionByTitle("Salary");

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(result[0].getAmount(), 5000.0);
    EXPECT_EQ(result[0].getCategory(), "Job");
    EXPECT_EQ(result[0].getDate(), "2022-01-14");
    EXPECT_EQ(result[0].getType(), TransactionType::Income);
}

TEST_F(TransactionManagerTest, SearchByTitleMultiple)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    for (int i = 0; i < 3; i++)
    {
        Transaction transaction(
            "Salary" + std::to_string(i),
            5000.0,
            "Job",
            "2022-01-14",
            TransactionType::Income);

        manager.addTransaction(transaction);
    }

    Transaction transaction(
        "Salary2",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    manager.addTransaction(transaction);

    auto result = manager.searchTransactionByTitle("Salary2");

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].getTitle(), "Salary2");
    EXPECT_EQ(result[1].getTitle(), "Salary2");
}

TEST_F(TransactionManagerTest, SearchByTitleThrowsWhenEmpty)
{
    Storage storage(filename);
    TransactionManager manager(storage);

    EXPECT_THROW(manager.searchTransactionByTitle("Title"), std::runtime_error);
}

TEST_F(TransactionManagerTest, SearchByTitleReturnsEmptyWhenNoMatch)
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

    EXPECT_EQ(manager.searchTransactionByTitle("Title").size(), 0);
}

TEST_F(TransactionManagerTest, SearchByTitleThrowsWhenEmptyTitle)
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

    EXPECT_THROW(manager.searchTransactionByTitle(""), std::invalid_argument);
}
