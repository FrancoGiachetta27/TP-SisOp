#ifndef SRC_INSTRUCTION_MEMORY_INSTRUCTION_INSTRUCTION_H_
#define SRC_INSTRUCTION_MEMORY_INSTRUCTION_INSTRUCTION_H_

#include <commons/collections/queue.h>
#include <initial_configuration/memory_config.h>


void fetch_next_instruccion(int pid, int program_pointer);
t_queue *get_instructions_from_file(t_log *logger, char *file_name);

#endif /* SRC_INSTRUCTION_MEMORY_INSTRUCTION_INSTRUCTION_H_*/