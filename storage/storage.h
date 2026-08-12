#pragma once

#include "../transaction/transaction.h"
#include <vector>
#include <string>

class Storage
{
public:
    Storage(const std::string &filename);

    void save(const std::vector<Transaction> &transactions);
    std::vector<Transaction> load();
    Transaction parseLine(const std::string &line);

private:
    std::string filename;
};