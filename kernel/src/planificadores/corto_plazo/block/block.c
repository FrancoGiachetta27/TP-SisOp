#include "block.h"

void unblock_processes(t_log* logger) {
    while (working)
    {
        sem_wait(&freed_resource);
	    void* _send_blocked_pcb_to_ready(char* _, t_block* block) {
            if (block->instances >= 1 && block->blocked_list->elements_count >= 1) {
                t_pcb* pcb = list_remove(block->blocked_list, 0);
                use_instance_of_resource(pcb, block, logger);
                sem_wait(&grd_mult);
                agregar_pcb_a_cola_READY(pcb, logger);
            }
        };
        dictionary_iterator(colas_BLOCKED, _send_blocked_pcb_to_ready);
    }
    sem_post(&planificadores_terminados);
}
