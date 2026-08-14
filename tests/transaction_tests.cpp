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
