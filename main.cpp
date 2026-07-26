#include "cli/cli.h"
#include "transaction_manager/transaction_manager.h"

int main()
{
    TransactionManager manager;

    CLI cli(manager);
    cli.run();

    return 0;
}