#include "cli/cli.h"
#include "transaction_manager/transaction_manager.h"

int main()
{
    Storage storage("transactions.csv");
    TransactionManager manager(storage);

    CLI cli(manager);
    cli.run();

    return 0;
}