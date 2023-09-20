#ifndef SRC_INSTRUCTIONS_DECODE_H_
#define SRC_INSTRUCTIONS_DECODE_H_

#include <string.h>
#include <commons/log.h>
// #include <commons/string.h>
#include <commons/collections/list.h>

typedef struct {
    int page_number;
    int displacement;
} t_pag;

typedef struct {
    char* instruction;
    t_list* params;
} t_ins;

t_ins decode(char* instruction, int page_size, t_log* logger);
t_pag* mmu_translate(char* logic_direction, int page_size);
void destroy_instruction(t_ins ins);

#endif /* SRC_INSTRUCTIONS_DECODE_H_ */