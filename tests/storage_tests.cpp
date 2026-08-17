#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "../storage/storage.h"

class StorageTest : public ::testing::Test
{
protected:
    const std::string filename = "storage_test.csv";
    Storage storage{filename};

    void SetUp() override
    {
        std::filesystem::remove(filename);
    }

    void TearDown() override
    {
        std::filesystem::remove(filename);
    }
};

TEST_F(StorageTest, ParsesValidIncomeLine)
{
    Transaction transaction = storage.parseLine(
        "Salary;5000;Job;2022-01-14;Income");

    EXPECT_EQ(transaction.getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 5000.0);
    EXPECT_EQ(transaction.getCategory(), "Job");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Income);
}

TEST_F(StorageTest, ParsesValidExpenseLine)
{
    Transaction transaction = storage.parseLine(
        "Groceries;200;Food;2022-01-14;Expense");

    EXPECT_EQ(transaction.getTitle(), "Groceries");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 200.0);
    EXPECT_EQ(transaction.getCategory(), "Food");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Expense);
}

TEST_F(StorageTest, RejectsInvalidTransactionType)
{
    EXPECT_THROW(
        storage.parseLine(
            "Salary;5000;Job;2022-01-14;Something"),
        std::runtime_error);
}

TEST_F(StorageTest, RejectsInvalidAmount)
{
    EXPECT_THROW(
        storage.parseLine(
            "Salary;abc;Job;2022-01-14;Income"),
        std::invalid_argument);
}

TEST_F(StorageTest, RejectsEmptyLine)
{
    EXPECT_THROW(
        storage.parseLine(""),
        std::invalid_argument);
}

TEST_F(StorageTest, SavesTransactionToFile)
{
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
    ASSERT_TRUE(std::getline(file, line));

    EXPECT_EQ(line, "Salary;5000;Job;2022-01-14;Income");
}

TEST_F(StorageTest, SavesMultipleTransactionsToFile)
{
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
}

TEST_F(StorageTest, LoadsTransactionsFromFile)
{
    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());

        file << "Salary;5000;Job;2022-01-14;Income\n";
        file << "Groceries;200;Food;2022-01-14;Expense\n";
    }

    std::vector<Transaction> transactions = storage.load();

    ASSERT_EQ(transactions.size(), 2);
    EXPECT_EQ(transactions[0].getTitle(), "Salary");
    EXPECT_EQ(transactions[1].getTitle(), "Groceries");
}

TEST_F(StorageTest, LoadsEmptyFile)
{
    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());
    }

    std::vector<Transaction> transactions = storage.load();

    EXPECT_TRUE(transactions.empty());
}

TEST(StorageFileTest, ThrowsWhenFileDoesNotExist)
{
    const std::string filename = "file_that_does_not_exist.csv";

    std::filesystem::remove(filename);

    Storage storage(filename);

    EXPECT_THROW(
        storage.load(),
        std::runtime_error);
}

TEST_F(StorageTest, IgnoresEmptyLines)
{
    {
        std::ofstream file(filename);

        ASSERT_TRUE(file.is_open());

        file << "Salary;5000;Job;2022-01-14;Income\n";
        file << "\n";
        file << "Groceries;200;Food;2022-01-14;Expense\n";
    }

    std::vector<Transaction> transactions = storage.load();

    ASSERT_EQ(transactions.size(), 2);

    EXPECT_EQ(transactions[0].getTitle(), "Salary");
    EXPECT_EQ(transactions[1].getTitle(), "Groceries");
}