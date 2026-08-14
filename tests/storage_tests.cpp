#include <gtest/gtest.h>
#include <filesystem>

#include "../storage/storage.h"

TEST(StorageTest, ParsesValidIncomeLine)
{
    std::filesystem::remove("storage_test.csv");

    Storage storage("storage_test.csv");

    Transaction transaction = storage.parseLine(
        "Salary;5000;Job;2022-01-14;Income");

    EXPECT_EQ(transaction.getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 5000.0);
    EXPECT_EQ(transaction.getCategory(), "Job");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Income);

    std::filesystem::remove("storage_test.csv");
}

TEST(StorageTest, ParsesValidExpenseLine)
{
    std::filesystem::remove("storage_test.csv");

    Storage storage("storage_test.csv");

    Transaction transaction = storage.parseLine(
        "Groceries;200;Food;2022-01-14;Expense");

    EXPECT_EQ(transaction.getTitle(), "Groceries");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 200.0);
    EXPECT_EQ(transaction.getCategory(), "Food");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Expense);

    std::filesystem::remove("storage_test.csv");
}

TEST(StorageTest, RejectsInvalidTransactionType)
{
    std::filesystem::remove("storage_test.csv");

    Storage storage("storage_test.csv");

    EXPECT_THROW(
        storage.parseLine(
            "Salary;5000;Job;2022-01-14;Something"),
        std::runtime_error);

    std::filesystem::remove("storage_test.csv");
}

TEST(StorageTest, RejectsInvalidAmount)
{
    std::filesystem::remove("storage_test.csv");

    Storage storage("storage_test.csv");

    EXPECT_THROW(
        storage.parseLine(
            "Salary;abc;Job;2022-01-14;Income"),
        std::invalid_argument);

    std::filesystem::remove("storage_test.csv");
}