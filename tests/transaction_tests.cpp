#include <gtest/gtest.h>
#include <stdexcept>

#include "../transaction/transaction.h"

TEST(TransactionTest, AcceptsValidDate)
{
    EXPECT_NO_THROW(
        Transaction(
            "Salary",
            5000.0,
            "Job",
            "2022-01-14",
            TransactionType::Income));
}

TEST(TransactionTest, RejectsInvalidDate)
{
    EXPECT_THROW(
        Transaction(
            "Salary",
            5000.0,
            "Job",
            "2022/01/14",
            TransactionType::Income),
        std::invalid_argument);
}

TEST(TransactionTest, ReturnsCorrectDate)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_EQ(transaction.getDate(), "2022-01-14");
}

TEST(TransactionTest, RejectsInvalidMonth)
{
    EXPECT_THROW(
        Transaction(
            "Salary",
            5000.0,
            "Job",
            "2022-13-14",
            TransactionType::Income),
        std::invalid_argument);
}

TEST(TransactionTest, RejectsInvalidDay)
{
    EXPECT_THROW(
        Transaction(
            "Salary",
            5000.0,
            "Job",
            "2022-01-32",
            TransactionType::Income),
        std::invalid_argument);
}

TEST(TransactionTest, RejectsDateWithWrongLength)
{
    EXPECT_THROW(
        Transaction(
            "Salary",
            5000.0,
            "Job",
            "2022-1-14",
            TransactionType::Income),
        std::invalid_argument);
}

TEST(TransactionTest, ReturnsCorrectTitle)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_EQ(transaction.getTitle(), "Salary");
}

TEST(TransactionTest, ReturnsCorrectAmount)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_DOUBLE_EQ(transaction.getAmount(), 5000.0);
}

TEST(TransactionTest, ReturnsCorrectCategory)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_EQ(transaction.getCategory(), "Job");
}

TEST(TransactionTest, ReturnsCorrectType)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_EQ(transaction.getType(), TransactionType::Income);
}

TEST(TransactionTest, ReturnsIncomeTypeString)
{
    Transaction transaction(
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income);

    EXPECT_EQ(transaction.getTypeString(), "Income");
}

TEST(TransactionTest, ReturnsExpenseTypeString)
{
    Transaction transaction(
        "Groceries",
        200.0,
        "Food",
        "2022-01-14",
        TransactionType::Expense);

    EXPECT_EQ(transaction.getTypeString(), "Expense");
}