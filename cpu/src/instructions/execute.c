#include <instructions/execute.h>

int execute(t_pcb* pcb, t_conn* conn, int memory_socket, t_reg* registers, t_ins ins, t_log* logger) {
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
        char* reg = list_get(ins.params, 0);
        char* new_program_counter = list_get(ins.params, 1);
        log_info(logger, "PID: %d - Ejecutando: JNZ - %s, %s", pcb->pid, reg, new_program_counter);
        jnz(pcb, registers, reg, new_program_counter);
        destroy_instruction(ins);
        return CONTINUE_NO_MODIFY_PROGRAM_COUNTER;
    } else if (strcmp(ins.instruction, "SLEEP")==0) {
        char* sleep_time = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: SLEEP - %s", pcb->pid, sleep_time);
        pcb->instruccion = SLEEP;
        pcb->params = atoi(sleep_time);
        pcb->programCounter++;
        destroy_instruction(ins);
        return RETURN_CONTEXT;
    } else if (strcmp(ins.instruction, "WAIT")==0) {
        char* resource = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: WAIT - %s", pcb->pid, resource);
        pcb->instruccion = WAIT;
        pcb->params = string_duplicate(resource);
        pcb->programCounter++;
        destroy_instruction(ins);
        return RETURN_CONTEXT;
    } else if (strcmp(ins.instruction, "SIGNAL")==0) {
        char* resource = list_get(ins.params, 0);
        log_info(logger, "PID: %d - Ejecutando: SIGNAL - %s", pcb->pid, resource);
        pcb->instruccion = SIGNAL;
        pcb->params = string_duplicate(resource);
        pcb->programCounter++;
        destroy_instruction(ins);
        return RETURN_CONTEXT;
    } else if (strcmp(ins.instruction, "MOV_IN")==0) {
        char* reg = list_get(ins.params, 0);
        t_pag* page = list_get(ins.params, 1);
        log_info(logger, "PID: %d - Ejecutando: MOV_IN - %s, %s", pcb->pid, reg, ins.original_address);
        pcb->instruccion = NORMAL;
        send_page(MOV_IN_COMMAND, page, memory_socket, logger);
        int op_code = receive_op_code(memory_socket, logger);
        if (op_code != MOV_IN_COMMAND) {
            log_info(logger, "Invalid op_code for MOV_IN: %d", op_code);
        }
        free(op_code);
        void* buffer = receive_buffer(memory_socket, logger);
        uint32_t value_in_frame = (uint32_t) buffer;
        free(buffer);
        uint32_t* selected_register = select_register(registers, reg);
        *selected_register = value_in_frame;
        log_info(logger, "PID: %d - Acción: LEER - Dirección Física: [%d, %d] - Valor: %d", pcb->pid, page->page_number, page->displacement, value_in_frame);
        free(ins.original_address);
    } else if (strcmp(ins.instruction, "MOV_OUT")==0) {
        t_pag* page = list_get(ins.params, 0);
        char* reg = list_get(ins.params, 1);
        uint32_t* register_value = select_register(registers, reg);
        log_info(logger, "PID: %d - Ejecutando: MOV_OUT - %s, %s", pcb->pid, reg, ins.original_address);
        pcb->instruccion = NORMAL;
        t_mov_out* mov_out_page = page_for_mov_out(page->pid, page->page_number, page->displacement, *register_value);
        send_page_for_mov_out(MOV_OUT_COMMAND, mov_out_page, memory_socket, logger);
        int op_code = receive_op_code(memory_socket, logger);
        if (op_code != MOV_OUT_COMMAND) {
            log_info(logger, "Invalid op_code for MOV_OUT: %d", op_code);
        }
        int result = receive_buffer(memory_socket, logger);
        if (result != 0) {
            log_info(logger, "Invalid result for MOV_OUT: %d", result);
        }
        free(result);
        free(mov_out_page);
        log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: [%d, %d] - Valor: %d", pcb->pid, page->page_number, page->displacement, *register_value);
        free(ins.original_address);
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
        return RETURN_CONTEXT;
    } else {
        log_warning(logger, "Invalid instruction %s", ins.instruction);
    }
    destroy_instruction(ins);
    return CONTINUE;
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

void jnz(t_pcb* pcb, t_reg* registers, char* reg, char* new_program_counter) {
    uint32_t* selected_reg = select_register(registers, reg);
    if (*selected_reg != 0) {
        pcb->programCounter = atoi(new_program_counter);
    }
}