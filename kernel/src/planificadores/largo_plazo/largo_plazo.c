#include "largo_plazo.h"

void atender_procesos_en_EXIT(t_log* logger) {
	while(working) {
        sem_wait(&process_in_exit);
        if (lista_estado_EXIT->elements_count != 0) {
            pthread_mutex_lock(&cola_exit);
            t_pcb* pcb = list_remove(lista_estado_EXIT, 0);
            pthread_mutex_unlock(&cola_exit);
            switch (pcb->end_state)
            {
            case SUCCESS:
                log_info(logger, "Finaliza el proceso %d - Motivo: SUCCESS", pcb->pid);
                break;
            case INVALID_RESOURCE:
                log_info(logger, "Finaliza el proceso %d - Motivo: INVALID_RESOURCE", pcb->pid);
                break;
            case INVALID_WRITE:
                log_info(logger, "Finaliza el proceso %d - Motivo: INVALID_WRITE", pcb->pid);
            default:
                log_error(logger, "Finaliza el proceso %d - Motivo: No deberia de haber llegado aca!", pcb->pid);
                break;
            }
            eliminar_proceso(pcb);
        }
	}
    sem_post(&planificadores_terminados);
}

void planificador_largo_plazo(t_log* logger) {
    pthread_t atender_exit;
    pthread_create(&atender_exit, NULL, (void*)atender_procesos_en_EXIT, logger);
    pthread_detach(atender_exit);
    while(working) {
        if (lista_estado_NEW->elements_count != 0) {
            sem_wait(&grd_mult);
            pthread_mutex_lock(&cola_exit);
            t_pcb* pcb_a_ready = list_remove(lista_estado_NEW, 0);
            pthread_mutex_unlock(&cola_exit);
            log_info(logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb_a_ready->pid, NEW, READY);
		    agregar_pcb_a_cola_READY(pcb_a_ready, logger);
        }
    }
    sem_post(&planificadores_terminados);
}

void iniciar_planificador_largo_plazo(t_log* logger) {
    pthread_t largo_plazo_hilo;
    pthread_create(&largo_plazo_hilo, NULL, (void*)planificador_largo_plazo, logger);
    pthread_detach(largo_plazo_hilo);
}