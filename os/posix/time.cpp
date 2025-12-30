#include "tx_api.h"


int usleep(useconds_t us)
{
    tx_thread_sleep(us / 1000);
    return 0;
}