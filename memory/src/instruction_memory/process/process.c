#include <instruction_memory/process/process.h>

void create_process(t_log *logger, int pid, char *file_name)
{
    t_list *instructions_set;

    if (instructions_set = get_instructions_from_file(logger, file_name) != NULL)
    {
        t_process process;

        process.pid = pid;
        process.file_name = file_name;
        process.instructions_set = instructions_set;

        list_add(active_processes, &process);
    }
    else
    {
        log_error(logger, "Error al crear el nuevo proceso");
    };
}

void deallocate_process(void) {}