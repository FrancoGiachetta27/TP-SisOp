#include "execute.h"

void execute_process(t_planificador* info) {
    while(1) {
        sem_wait(&executing_process);
        if (!working) break;
        if (estado_EXEC != NULL) {
            send_pcb(INSTRUCTION, estado_EXEC, info->conn->cpu_dispatcher_socket, info->utils->logger);
            int op_code = receive_op_code(info->conn->cpu_dispatcher_socket, info->utils->logger);
            if (op_code != EXECUTED_INSTRUCTION) log_warning(info->utils->logger, "Received incorrect message %d", op_code);
            t_pcb* pcb = receive_pcb(info->conn->cpu_dispatcher_socket, info->utils->logger);
            switch (pcb->instruccion)
            {
                case INTERRUPTED:
                case SLEEP:
                    destroy_executing_process();
                    interrupt_process(pcb, info->utils->logger);
                break;
                case INTERRUPT_FINISH:
                    sem_post(&finish_interrupted_process);
                break;
                case FINISH:
                    destroy_executing_process();
                    send_to_exit(pcb, info->utils->logger, SUCCESS);
                break;
                case WAIT:
                    if (!dictionary_has_key(colas_BLOCKED, pcb->params)) {
                        destroy_executing_process();
                        send_to_exit(pcb, info->utils->logger, INVALID_RESOURCE);
                        break;
                    }
                    wait_instance_of_resource(pcb, info->utils->logger);
                break;
                case SIGNAL:
                    if (!dictionary_has_key(colas_BLOCKED, pcb->params)) {
                        destroy_executing_process();
                        send_to_exit(pcb, info->utils->logger, INVALID_RESOURCE);
                        break;
                    }
                    signal_instance_of_resource(pcb, info->utils->logger);
                break;
            }
        }
    }
    free(info);
}