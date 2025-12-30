#include "tx_api.h"
#include <sys/types.h>

extern "C" {

int usleep(unsigned int us)
{
    tx_thread_sleep(us / 1000);
    return 0;
}

}  // extern "C"