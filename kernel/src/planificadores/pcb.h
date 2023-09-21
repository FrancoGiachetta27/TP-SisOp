/* 

#include <commons/collections/list.h>



*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <commons/string.h>
#include <config/config.h>
#include <pthread.h>
#include <initial_configuration/client_start.h>


typedef struct pcb{

    uint32_t pid;
    uint32_t tamanio;
    uint32_t programCounter;
    uint32_t prioridad;
    char* nom_arch_inst;

}t_pcb;

pthread_mutex_t siguientePIDmutex;

t_pcb* crear_pcb(uint32_t pid, uint32_t tamanio, uint32_t prioridad);
