#include "corto_plazo.h"

void planificador_fifo(t_planificador* info) {
    while(working) {
        if (estado_EXEC == NULL && lista_estado_READY->elements_count != 0) {
            pthread_mutex_lock(&estados_mutex);
            t_pcb* pcb = list_remove(lista_estado_READY, 0);
            log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, READY, EXEC);
            pcb->estado = EXEC;
            estado_EXEC = pcb;
	        pthread_mutex_unlock(&estados_mutex);
            sem_post(&grd_mult);
        }
        if (estado_EXEC != NULL) {
            send_pcb(INSTRUCTION, estado_EXEC, info->conn->cpu_dispatcher_socket, info->utils->logger);
            int op_code = receive_op_code(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (op_code != EXECUTED_INSTRUCTION) log_warning(info->utils->logger, "Received incorrect message %d", op_code);
            t_pcb* pcb = receive_pcb(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (pcb->programCounter == -1) {
                pthread_mutex_lock(&estados_mutex);
                destroy_pcb(estado_EXEC);
                estado_EXEC = NULL;
                log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, EXEC, EXIT);
                list_add(lista_estado_EXIT, pcb);
                pthread_mutex_unlock(&estados_mutex);
            } else {
                pthread_mutex_lock(&estados_mutex);
                destroy_pcb(estado_EXEC);
                estado_EXEC = pcb;
                pthread_mutex_unlock(&estados_mutex);
            }
        }
    }
    free(info);
    sem_post(&planificadores_terminados);
}

void planificador_rr(t_planificador* info) {
}

void planificador_prioridades(t_planificador* info) {
}

void iniciar_planificador_corto_plazo(t_utils* utils, t_conn* conn) {
    pthread_t hilo_planificador_corto_plazo;
    t_planificador* planificador_info = malloc(sizeof(*planificador_info));
    planificador_info->conn = conn;
    planificador_info->utils = utils;
    char* algoritmo = config_get_string_value(utils->config, "ALGORITMO_PLANIFICACION");
    log_info(utils->logger,"Usando algoritmo: %s", algoritmo);
    if (strcmp(algoritmo,"FIFO")==0) {
        pthread_create(&hilo_planificador_corto_plazo, NULL, (void*)planificador_fifo, planificador_info);
    }
    if (strcmp(algoritmo,"RR")==0) {
        pthread_create(&hilo_planificador_corto_plazo, NULL, (void*)planificador_rr, planificador_info);
    }
    if (strcmp(algoritmo,"PRIORIDADES")==0) {
        pthread_create(&hilo_planificador_corto_plazo, NULL, (void*)planificador_prioridades, planificador_info);
    }
    pthread_detach(hilo_planificador_corto_plazo);
}