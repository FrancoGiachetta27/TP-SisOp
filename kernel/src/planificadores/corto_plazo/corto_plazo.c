#include "corto_plazo.h"

void sleep_process(t_sleep* sleep_info) {
    sleep(sleep_info->pcb->params);
    sem_wait(&grd_mult);
    agregar_pcb_a_cola_READY(sleep_info->pcb, sleep_info->logger);
    free(sleep_info);
}

void execute_process(t_planificador* info) {
    while(working) {
        sem_wait(&executing_process);
        if (estado_EXEC != NULL) {
            send_pcb(INSTRUCTION, estado_EXEC, info->conn->cpu_dispatcher_socket, info->utils->logger);
            int op_code = receive_op_code(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (op_code != EXECUTED_INSTRUCTION) log_warning(info->utils->logger, "Received incorrect message %d", op_code);
            t_pcb* pcb = receive_pcb(info->conn->cpu_dispatcher_socket, info->utils->logger);
            switch (pcb->instruccion)
            {
                case FINISH:
                    pthread_mutex_lock(&mtx_execute_process);
                    destroy_pcb(estado_EXEC);
                    estado_EXEC = NULL;
                    pthread_mutex_unlock(&mtx_execute_process);
                    log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, EXEC, EXIT);
                    list_add(lista_estado_EXIT, pcb);
                    sem_post(&proceso_en_cola_ready);
                break;
                case SLEEP:
                    pthread_mutex_lock(&mtx_execute_process);
                    destroy_pcb(estado_EXEC);
                    estado_EXEC = NULL;
                    pthread_mutex_unlock(&mtx_execute_process);
                    pcb->estado = BLOCKED;
                    log_info(info->utils->logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, EXEC, BLOCKED);
                    pthread_t sleep_process_thread;
                    t_sleep* sleep_info = malloc(sizeof(*sleep_info));
                    sleep_info->pcb = pcb;
                    sleep_info->logger = info->utils->logger;
                    pthread_create(&sleep_process_thread, NULL, (void*)sleep_process, sleep_info);
                    pthread_detach(sleep_process_thread);
                break;
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
            pthread_mutex_lock(&mtx_execute_process);
            estado_EXEC = pcb;
            pthread_mutex_unlock(&mtx_execute_process);
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