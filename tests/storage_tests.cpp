#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "../storage/storage.h"

TEST(StorageTest, ParsesValidIncomeLine)
{
    Storage storage("unused.csv");

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
    Storage storage("unused.csv");

    Transaction transaction = storage.parseLine(
        "Groceries;200;Food;2022-01-14;Expense");

    EXPECT_EQ(transaction.getTitle(), "Groceries");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 200.0);
    EXPECT_EQ(transaction.getCategory(), "Food");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Expense);
}

TEST(StorageTest, RejectsInvalidTransactionType)
{
    Storage storage("unused.csv");

    EXPECT_THROW(
        storage.parseLine(
            "Salary;5000;Job;2022-01-14;Something"),
        std::runtime_error);
}

TEST(StorageTest, RejectsInvalidAmount)
{
    Storage storage("unused.csv");

    EXPECT_THROW(
        storage.parseLine(
            "Salary;abc;Job;2022-01-14;Income"),
        std::invalid_argument);
}

TEST(StorageTest, RejectsEmptyLine)
{
    Storage storage("unused.csv");

    EXPECT_THROW(
        storage.parseLine(
            ""),
        std::invalid_argument);
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

TEST(StorageTest, SavesMultipleTransactionsToFile)
{
    const std::string filename = "storage_test.csv";

    std::filesystem::remove(filename);

    Storage storage(filename);

    Transaction income(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    Transaction expense(
        "Groceries",
        200.0,
        "Food",
        "2022-01-14",
        TransactionType::Expense);

    storage.save({income, expense});

    std::ifstream file(filename);

    ASSERT_TRUE(file.is_open());

    std::string line;

    ASSERT_TRUE(std::getline(file, line));
    EXPECT_EQ(
        line,
        "Salary;5000;Job;2022-01-14;Income");

    ASSERT_TRUE(std::getline(file, line));
    EXPECT_EQ(
        line,
        "Groceries;200;Food;2022-01-14;Expense");

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

TEST(StorageTest, LoadsEmptyFile)
{
    const std::string filename = "storage_test.csv";

    std::filesystem::remove(filename);

    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());
    }

    Storage storage(filename);

    std::vector<Transaction> transactions = storage.load();

    EXPECT_TRUE(transactions.empty());

    std::filesystem::remove(filename);
}

TEST(StorageTest, ThrowsWhenFileDoesNotExist)
{
    const std::string filename = "file_that_does_not_exist.csv";

    std::filesystem::remove(filename);

    Storage storage(filename);

    EXPECT_THROW(
        storage.load(),
        std::runtime_error);
}

TEST(StorageTest, IgnoresEmptyLines)
{
    const std::string filename = "storage_test.csv";

    std::filesystem::remove(filename);

    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());

        file << "Salary;5000;Job;2022-01-14;Income\n";
        file << "\n";
        file << "Groceries;200;Food;2022-01-14;Expense\n";
    }

    Storage storage(filename);

    std::vector<Transaction> transactions = storage.load();

    ASSERT_EQ(transactions.size(), 2);

    EXPECT_EQ(transactions[0].getTitle(), "Salary");
    EXPECT_EQ(transactions[1].getTitle(), "Groceries");

    std::filesystem::remove(filename);
}