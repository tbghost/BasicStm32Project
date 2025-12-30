#include "posix.hpp"
#include "tx_api.h"
#include <cstdint>
#include <stdint.h>

/* Static thread stacks - in production, consider dynamic allocation */
static uint8_t pthread_stacks[16][2048];
static int pthread_count = 0;

extern "C" {

int pthread_create(pthread_t* t, void*, void (*fn)(void*), void* arg)
{
    uint8_t* stack = pthread_stacks[pthread_count];
    pthread_count++;
    
    return tx_thread_create(t,  
                            "pthread",
                            (void (*)(ULONG))fn,
                            (ULONG)arg,
                            stack,
                            sizeof(pthread_stacks[0]),
                            5, 
                            5, 
                            TX_NO_TIME_SLICE, 
                            TX_AUTO_START
                        );
}

}  // extern "C"