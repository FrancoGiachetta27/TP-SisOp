#include "largo_plazo.h"

void atender_procesos_en_EXIT(t_log* logger) {
	while(working) {
        if (lista_estado_EXIT->elements_count != 0) {
            t_pcb* pcb = list_get(lista_estado_EXIT, 0);
            eliminar_proceso(pcb);
        }
	}
}

void planificador_largo_plazo(t_log* logger) {
    pthread_t atender_exit;
    pthread_create(&atender_exit, NULL, (void*)atender_procesos_en_EXIT, logger);
    pthread_detach(atender_exit);
    while(working) {
        if (lista_estado_NEW->elements_count != 0) {
            sem_wait(&grd_mult);
            t_pcb* pcb_a_ready = list_remove(lista_estado_NEW, 0);
		    agregar_pcb_a_cola_READY(pcb_a_ready, logger);
        }
    }
}

void iniciar_planificador_largo_plazo(t_log* logger) {
    pthread_t largo_plazo_hilo;
    pthread_create(&largo_plazo_hilo, NULL, (void*)planificador_largo_plazo, logger);
    pthread_detach(largo_plazo_hilo);
}

void eliminar_proceso(t_pcb* pcb) {
    // Liberar instancias de archivos
    // Llamar a memoria para liberar
    destroy_pcb(pcb);
}