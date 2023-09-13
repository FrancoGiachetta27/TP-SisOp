#include <instruction_memory/instruction_memory.h>

void fetch_next_instruccion(int program_pointer) {}

void create_process(t_log *logger, char *file_name)
{
    get_instructions_from_file(logger, file_name);
}

static t_list *get_instructions_from_file(t_log *logger, char *file_name)
{
    char *file_path = string_from_format("%s/%s", memory_config.instructions_path, file_name);
    FILE *pseudo_code_file = fopen(file_path, "r");
    char *line = NULL;
    size_t instruction_len = 0;
    t_list *instructions = list_create();
    t_instruction instruction;

    while (getline(&line, &instruction_len, pseudo_code_file) != -1)
    {
        char **instruct_params = string_split(line, " ");

        instruction.op_code = instruct_params[1];
        instruction.params = queue_create();

        for (int i = 1; i < string_array_size(instruct_params); i++)
        {
            queue_push(instruction.params, instruct_params[i]);
        }

        string_array_destroy(instruct_params);
    }

    fclose(pseudo_code_file);

    return instructions;
}