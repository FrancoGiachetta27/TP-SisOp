#include "pcb.h"


uint32_t obt_sig_PID() {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = sig_PID++;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}
