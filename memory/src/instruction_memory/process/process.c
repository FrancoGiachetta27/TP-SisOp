#include <instruction_memory/process/process.h>

t_list* active_processes;

void create_process(t_log *logger, int pid, char *file_name, int bytes)
{
    char *file_path = string_from_format("%s/%s", memory_config.instructions_path, file_name);
    t_list *instructions_set = get_instructions_from_file(logger, file_path);

    if (instructions_set != NULL)
    {
        t_process* process;

        process->pid = pid;
        process->file_name = file_name;
        process->bytes = bytes;
        process->instructions_set = instructions_set;

        list_add(active_processes, process);

    }
    else
    {
        log_error(logger, "Error al crear el nuevo proceso");
    };
}

void deallocate_process(void) {}