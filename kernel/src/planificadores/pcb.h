#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

pthread_mutex_t siguientePIDmutex;

//extern uint32_t sig_PID;

uint32_t obtener_siguiente_pid(void);
