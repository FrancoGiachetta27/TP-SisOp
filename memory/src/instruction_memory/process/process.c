#include <instruction_memory/process/process.h>

t_list* active_processes;

int create_process(t_log *logger, int pid, char *file_name, int bytes)
{
    char *file_path = string_from_format("%s/%s", memory_config.instructions_path, file_name);
    t_list *instructions_set = get_instructions_from_file(logger, file_path);

    if (instructions_set == NULL) return -1;
    
    t_process* process;

    process->pid = pid;
    process->file_name = file_name;
    process->bytes = bytes;
    process->instructions_set = instructions_set;

        list_add(active_processes, process);

        return 1;
}

void deallocate_process(void) {}