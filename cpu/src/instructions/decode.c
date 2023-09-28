#include <instructions/decode.h>

t_ins decode(char* instruction, int page_size, t_log* logger) {
    char** formatted_ins = string_split(instruction, " ");
    t_ins ins;
    ins.instruction = formatted_ins[0];
    ins.params = list_create();
    for (int i = 1; i < string_array_size(formatted_ins); i++)
    {
        list_add(ins.params, formatted_ins[i]);
    }
    if (strcmp(ins.instruction, "MOV_IN")==0 || strcmp(ins.instruction, "F_READ")==0 || strcmp(ins.instruction, "F_WRITE")==0) {
        char* logic_direction = list_get(ins.params, 1);
        t_pag* physical_direction = mmu_translate(logic_direction, page_size);
        list_replace(ins.params, 1, physical_direction);
    } else if (strcmp(ins.instruction, "MOV_OUT")==0)
    {
        char* logic_direction = list_get(ins.params, 0);
        t_pag* physical_direction = mmu_translate(logic_direction, page_size);
        list_replace(ins.params, 0, physical_direction);
    }
    free(formatted_ins);
    return ins;
}

t_pag* mmu_translate(char* logic_direction, int page_size) {
    t_pag* physical_direction = malloc(sizeof(t_pag*));
    int int_logic_direction = atoi(logic_direction);
    physical_direction->page_number = floor(int_logic_direction / page_size);
    physical_direction->displacement = int_logic_direction - physical_direction->page_number * page_size;
    free(logic_direction);
    return physical_direction;
}

void destroy_instruction(t_ins ins) {
    list_destroy(ins.params);
}