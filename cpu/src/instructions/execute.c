#include <instructions/execute.h>

int execute(t_pcb* pcb, t_conn* conn, t_reg* registers, t_ins ins, t_log* logger) {
    destroy_params(pcb);
    if (strcmp(ins.instruction,"SET")==0) {
        pcb->instruccion = NORMAL;
        char* reg = list_get(ins.params, 0);
        char* value = list_get(ins.params, 1);
        log_info(logger, "PID: %d - Ejecutando: SET - %s, %s", pcb->pid, reg, value);
        set(registers, reg, value);
    } else if (strcmp(ins.instruction, "SUM")==0) {
        pcb->instruccion = NORMAL;
        char* destination_reg = list_get(ins.params, 0);
        char* origin_reg = list_get(ins.params, 1);
        log_info(logger, "PID: %d - Ejecutando: SUM - %s, %s", pcb->pid, destination_reg, origin_reg);
        sum(registers, destination_reg, origin_reg);
    } else if (strcmp(ins.instruction, "SUB")==0) {
        pcb->instruccion = NORMAL;
        char* destination_reg = list_get(ins.params, 0);
        char* origin_reg = list_get(ins.params, 1);
        log_info(logger, "PID: %d - Ejecutando: SUB - %s, %s", pcb->pid, destination_reg, origin_reg);
        sub(registers, destination_reg, origin_reg);
    } else if (strcmp(ins.instruction, "JNZ")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "SLEEP")==0) {
        char* sleep_time = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: SLEEP - %s", pcb->pid, sleep_time);
        pcb->instruccion = SLEEP;
        pcb->params = atoi(sleep_time);
        pcb->programCounter++;
        destroy_instruction(ins);
        return -1;
    } else if (strcmp(ins.instruction, "WAIT")==0) {
        char* resource = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: WAIT - %s", pcb->pid, resource);
        pcb->instruccion = WAIT;
        pcb->params = string_duplicate(resource);
        pcb->programCounter++;
        destroy_instruction(ins);
        return -1;
    } else if (strcmp(ins.instruction, "SIGNAL")==0) {
        char* resource = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: SIGNAL - %s", pcb->pid, resource);
        pcb->instruccion = SIGNAL;
        pcb->params = string_duplicate(resource);
        pcb->programCounter++;
        destroy_instruction(ins);
        return -1;
    } else if (strcmp(ins.instruction, "MOV_IN")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "MOV_OUT")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_OPEN")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_CLOSE")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_SEEK")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_READ")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_WRITE")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "F_TRUNCATE")==0) {
        pcb->instruccion = NORMAL;
    } else if (strcmp(ins.instruction, "EXIT")==0) {
        log_info(logger, "PID: %d - Ejecutando: EXIT", pcb->pid);
        pcb->instruccion = FINISH;
        destroy_instruction(ins);
        return -1;
    } else {
        log_warning(logger, "Invalid instruction %s", ins.instruction);
    }
    destroy_instruction(ins);
    return 0;
}

uint32_t* select_register(t_reg* registers, char* string_register) {
    uint32_t* final_register;
    if (string_equals_ignore_case(string_register, "AX")) {
        final_register = &registers->AX;
    } else if (string_equals_ignore_case(string_register, "BX")) {
        final_register = &registers->BX;
    } else if (string_equals_ignore_case(string_register, "CX")) {
        final_register = &registers->CX;
    } else if (string_equals_ignore_case(string_register, "DX")) {
        final_register = &registers->DX;
    }
    return final_register;
}

void set(t_reg* registers, char* reg, char* value) {
    uint32_t value_int = (uint32_t)atoi(value);
    uint32_t* selected_register = select_register(registers, reg);
    *selected_register = value_int;
}

void sum(t_reg* registers, char* destination_reg, char* origin_reg) {
    uint32_t* selected_destination_reg = select_register(registers, destination_reg);
    uint32_t* selected_origin_reg = select_register(registers, origin_reg);
    *selected_destination_reg = *selected_destination_reg + *selected_origin_reg;
}

void sub(t_reg* registers, char* destination_reg, char* origin_reg) {
    uint32_t* selected_destination_reg = select_register(registers, destination_reg);
    uint32_t* selected_origin_reg = select_register(registers, origin_reg);
    *selected_destination_reg = *selected_destination_reg - *selected_origin_reg;
}