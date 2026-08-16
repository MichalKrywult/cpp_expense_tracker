#include <gtest/gtest.h>
#include <stdexcept>

#include "../transaction/transaction.h"

class TransactionTestFixture : public ::testing::Test
{
protected:
    Transaction transaction{
        "Salary",
        5000.0,
        "Job",
        "2022-01-14",
        TransactionType::Income};
};

TEST_F(TransactionTestFixture, ReturnsCorrectProperties)
{
    EXPECT_EQ(transaction.getTitle(), "Salary");
    EXPECT_DOUBLE_EQ(transaction.getAmount(), 5000.0);
    EXPECT_EQ(transaction.getCategory(), "Job");
    EXPECT_EQ(transaction.getDate(), "2022-01-14");
    EXPECT_EQ(transaction.getType(), TransactionType::Income);
    EXPECT_EQ(transaction.getTypeString(), "Income");
}

TEST(TransactionTest, RejectsDateWithInvalidFormat)
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