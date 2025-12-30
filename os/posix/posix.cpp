#include "posix.hpp"
#include "tx_api.h"


int pthread_create(pthread_t* t, void*, void (*fn)(void*), void* arg)
{
    return tx_thread_create(t,  
                            "pthread",
                            (void (*)(ULONG))fn,
                            (ULONG)arg,
                            stack,
                            sizeof(stack),
                            5, 
                            5, 
                            TX_NO_TIME_SLICE, 
                            TX_AUTO_START
                        );
}