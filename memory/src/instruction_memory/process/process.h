#ifndef SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS
#define SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS

#include <commons/log.h>
#include <commons/collections/list.h>
#include <instruction_memory/instructions/instructions.h>
#include <user_memory/paging/page_table.h>

typedef struct
{
    int pid;
    char* file_name;
    int bytes;
    t_list* instructions_set;
    t_list* page_table;
} t_process;

int create_process(t_log *logger, int pid, char *file_name, int bytes, int swap_blocks);
void deallocate_porcess(int pid);

extern t_list* active_processes;

#endif /* SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS */