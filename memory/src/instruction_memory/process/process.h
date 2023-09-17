#ifndef SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS
#define SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS

#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

typedef struct
{
    int pid;
    char *file_name;
    t_queue *instructions_set;
} t_process;

void create_process(t_log *logger, int pid, char *file_name);
void deallocate_porcess(void);

extern t_list* active_processes;

#endif /* SRC_INSTRUCTION_MEMORY_PROCESS_PROCESS */