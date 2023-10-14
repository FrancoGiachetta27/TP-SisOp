#include "fifo.h"

void planificador_fifo(t_planificador* info) {
    while(working) {
        sem_wait(&proceso_en_cola_ready);
        if (estado_EXEC == NULL && lista_estado_READY->elements_count != 0) {
            pthread_mutex_lock(&cola_ready);
            t_pcb* pcb = list_remove(lista_estado_READY, 0);
            pthread_mutex_unlock(&cola_ready);
            execute_ready_process(pcb, info->utils->logger);
        }
    }
    free(info);
    sem_post(&planificadores_terminados);
}