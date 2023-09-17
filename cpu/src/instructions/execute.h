#ifndef SRC_INSTRUCTIONS_EXECUTE_H_
#define SRC_INSTRUCTIONS_EXECUTE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
// #include <commons/string.h>

typedef struct {
    uint32_t AX;
    uint32_t BX;
    uint32_t CX;
    uint32_t DX;
} t_reg;

void execute(t_reg* registers, char** complete_instruction, t_log* logger);
uint32_t* select_register(t_reg* registers, char* reg);
void set(t_reg* registers, char* reg, char* value);
void sum(t_reg* registers, char* destination_reg, char* origin_reg);
void sub(t_reg* registers, char* destination_reg, char* origin_reg);

#endif /* SRC_INSTRUCTIONS_EXECUTE_H_ */