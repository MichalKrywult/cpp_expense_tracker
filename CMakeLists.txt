cmake_minimum_required(VERSION 4.0)

project(ExpenseTracker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
    ExpenseTracker
    main.cpp
    transaction/transaction.cpp
    transaction_manager/transaction_manager.cpp
    cli/cli.cpp
    storage/storage.cpp
)

target_include_directories(
    ExpenseTracker
    PRIVATE .
)

if(MSVC)
    target_compile_options(ExpenseTracker PRIVATE /W4)
else()
    target_compile_options(ExpenseTracker PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wswitch-enum
    )
endif()