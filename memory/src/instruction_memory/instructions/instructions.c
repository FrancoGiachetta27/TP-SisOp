#include <instruction_memory/instructions/instructions.h>

t_queue *get_instructions_from_file(t_log *logger, char *file_name)
{
    char *file_path = string_from_format("%s/%s", memory_config.instructions_path, file_name);
    FILE *pseudo_code_file = fopen(file_path, "r");

    if (pseudo_code_file == NULL)
    {
        log_error(logger, "Error al leer el archivo pseudo codigo");

        return NULL;
    }

    char *line = NULL;
    int instruction_len = 0;
    t_list *instructions = queue_create();

    while (getline(&line, &instruction_len, pseudo_code_file) != -1)
    {
        queue_push(instructions, line);
    }

    fclose(pseudo_code_file);

    return instructions;
}

void fetch_next_instruccion(int pid, int program_pointer)
{
}