#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

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

TEST(StorageTest, RejectsEmptyLine)
{
    std::filesystem::remove("storage_test.csv");

    Storage storage("storage_test.csv");

    EXPECT_THROW(
        storage.parseLine(
            ""),
        std::invalid_argument);

    std::filesystem::remove("storage_test.csv");
}

TEST(StorageTest, SavesTransactionToFile)
{
    const std::string filename = "storage_test.csv";

    std::filesystem::remove(filename);

    Storage storage(filename);

    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    storage.save({transaction});

    std::ifstream file(filename);

    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line);

    EXPECT_EQ(
        line,
        "Salary;5000;Job;2022-01-14;Income");

    file.close();

    std::filesystem::remove(filename);
}

TEST(StorageTest, LoadsTransactionsFromFile)
{
    const std::string filename = "storage_test.csv";

    std::filesystem::remove(filename);

    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());

        file << "Salary;5000;Job;2022-01-14;Income\n";
        file << "Groceries;200;Food;2022-01-14;Expense\n";
    }

    Storage storage(filename);

    std::vector<Transaction> transactions = storage.load();

    ASSERT_EQ(transactions.size(), 2);

    EXPECT_EQ(transactions[0].getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(transactions[0].getAmount(), 5000.0);
    EXPECT_EQ(transactions[0].getType(), TransactionType::Income);

    EXPECT_EQ(transactions[1].getTitle(), "Groceries");
    EXPECT_DOUBLE_EQ(transactions[1].getAmount(), 200.0);
    EXPECT_EQ(transactions[1].getType(), TransactionType::Expense);

    std::filesystem::remove(filename);
}