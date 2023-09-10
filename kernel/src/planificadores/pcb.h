#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <unistd.h>
#include <stdint.h>

typedef struct t_pcb {
    uint32_t pid;
    uint32_t tamanio;
    uint32_t programCounter;
    uint32_t prioridad;
    t_list* tablaDeArchivosAbiertos;
    pthread_mutex_t* mutex;

}t_pcb;
