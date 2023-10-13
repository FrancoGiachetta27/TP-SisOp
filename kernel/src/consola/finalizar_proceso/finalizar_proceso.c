#include "finalizar_proceso.h"

void finalizar_proceso(uint32_t pid) {
	t_pcb* pcb;
	if (estado_EXEC->pid == pid) {
		pthread_mutex_lock(&mtx_execute_process);
		destroy_pcb(estado_EXEC);
		estado_EXEC = NULL;
		pthread_mutex_unlock(&mtx_execute_process);
		sem_post(&proceso_en_cola_ready);
		eliminar_proceso(pcb);
		return;
	}

	bool _pcb_by_pid_in_list(t_pcb *pcb) {
        return pcb->pid == pid;
    };

	pcb = list_remove_by_condition(lista_estado_NEW, (void*) _pcb_by_pid_in_list);
	if (pcb != NULL) {
		eliminar_proceso(pcb);
		return;
	}
	pthread_mutex_lock(&cola_ready);
	pcb = list_remove_by_condition(lista_estado_READY, (void*) _pcb_by_pid_in_list);
    pthread_mutex_unlock(&cola_ready);
	if (pcb != NULL) {
		sem_post(&grd_mult);
		eliminar_proceso(pcb);
		return;
	}
	pthread_mutex_lock(&cola_exit);
	pcb = list_remove_by_condition(lista_estado_EXIT, (void*) _pcb_by_pid_in_list);
    pthread_mutex_unlock(&cola_exit);
	if (pcb != NULL) {
		eliminar_proceso(pcb);
		return;
	}

	void _pcb_by_pid_in_dict(char* _, t_block* block) {
		pcb = list_remove_by_condition(block->blocked_list, (void*) _pcb_by_pid_in_list);
		if (pcb != NULL) {
			eliminar_proceso(pcb);
		}
    };
	dictionary_iterator(colas_BLOCKED, (void*) _pcb_by_pid_in_dict);
};