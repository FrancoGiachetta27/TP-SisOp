#ifndef SRC_PLANIFICADORES_CODIGO_DE_OPERACION_H_
#define SRC_PLANIFICADORES_CODIGO_DE_OPERACION_H_

#include <stdlib.h>
#include <stdio.h>

enum KERNEL_COMMAND {
    CREATE_PROCESS = 2,
    EXECUTED_INSTRUCTION = 3,
    PROCESS_OK = 5
};

enum CPU_OP_CODES {
    INSTRUCTION = 3,
};

#endif /* SRC_PLANIFICADORES_CODIGO_DE_OPERACION_H_ */
