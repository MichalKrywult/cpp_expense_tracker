cmake_minimum_required(VERSION 4.0)

project(ExpenseTracker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ExpenseTracker
    main.cpp
    transaction/transaction.cpp
)

target_include_directories(
    ExpenseTracker
    PRIVATE .
)