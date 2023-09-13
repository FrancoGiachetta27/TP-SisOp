#ifndef SRC_INSTRUCTION_MEMORY_INSTRUCTION_MEMORY_H_
#define SRC_INSTRUCTION_MEMORY_INSTRUCTION_MEMORY_H_

#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <initial_configuration/memory_config.h>

typedef struct
{
    char *op_code;
    t_queue *params;
} t_instruction;

typedef struct
{
    int pid;
    char *file_name;
    t_list *instructions;
} t_process;

void create_process(t_log *logger, char *file_name);

#endif /* SRC_INSTRUCTION_MEMORY_INSTRUCTION_MEMORY_H_ */