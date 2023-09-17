#ifndef SRC_INSTRUCTIONS_DECODE_H_
#define SRC_INSTRUCTIONS_DECODE_H_

#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef struct {
    int page_number;
    int displacement;
} t_pag;

typedef struct {
    char* instruction;
    t_list* params;
    numero
} t_ins;

char** decode(char* instruction, int tamanio_pagina, int numero_pagina, t_log* logger);

#endif /* SRC_INSTRUCTIONS_DECODE_H_ */