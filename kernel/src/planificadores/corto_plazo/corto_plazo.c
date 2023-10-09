#include "corto_plazo.h"

void execute_process(t_planificador* info) {
    while(working) {
        sem_wait(&executing_process);
        if (estado_EXEC != NULL) {
            send_pcb(INSTRUCTION, estado_EXEC, info->conn->cpu_dispatcher_socket, info->utils->logger);
            int op_code = receive_op_code(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (op_code != EXECUTED_INSTRUCTION) log_warning(info->utils->logger, "Received incorrect message %d", op_code);
            t_pcb* pcb = receive_pcb(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (pcb->programCounter == -1) {
                destroy_pcb(estado_EXEC);
                estado_EXEC = NULL;
                log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, EXEC, EXIT);
                list_add(lista_estado_EXIT, pcb);
            }
        }
    }
    free(info);
    sem_post(&planificadores_terminados);
}

void planificador_fifo(t_planificador* info) {
    while(working) {
        sem_wait(&proceso_en_cola_ready);
        if (estado_EXEC == NULL && lista_estado_READY->elements_count != 0) {
            pthread_mutex_lock(&cola_ready);
            t_pcb* pcb = list_remove(lista_estado_READY, 0);
            pthread_mutex_unlock(&cola_ready);
            log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, READY, EXEC);
            pcb->estado = EXEC;
            estado_EXEC = pcb;
            sem_post(&executing_process);
            sem_post(&grd_mult);
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
    pthread_t hilo_execute;
    t_planificador* planificador_info = malloc(sizeof(*planificador_info));
    planificador_info->conn = conn;
    planificador_info->utils = utils;
    t_planificador* execute_info = malloc(sizeof(*execute_info));
    execute_info->conn = conn;
    execute_info->utils = utils;
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
    pthread_create(&hilo_execute, NULL, (void*)execute_process, execute_info);
    pthread_detach(hilo_execute);
}