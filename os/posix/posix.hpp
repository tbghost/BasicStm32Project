
#pragma once


using pthread_t = TX_THREAD;
int pthread_create(pthread_t*, void*, void (*)(void*), void*);
int usleep(unsigned int);