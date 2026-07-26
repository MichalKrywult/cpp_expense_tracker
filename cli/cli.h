#include "../transaction_manager/transaction_manager.h"

class CLI
{
private:
    TransactionManager &manager;

public:
    CLI(TransactionManager &manager);

    void run();
};