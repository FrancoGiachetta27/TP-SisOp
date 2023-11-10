#ifndef SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS
#define SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS

#include <commons/log.h>
#include <commons/collections/list.h>
#include <instruction_memory/instructions/instructions.h>
#include <user_memory/paging/pages.h>
#include <pcb/pcb.h>

typedef struct
{
    uint32_t pid;
    char* file_name;
    int bytes;
    t_list* instructions_set;
} t_process;

int create_process(t_log *logger, t_pcb* pcb, int swap_blocks);
t_process* search_process(int pid);
void deallocate_porcess(int pid);

extern t_list* active_processes;

#endif /* SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS */