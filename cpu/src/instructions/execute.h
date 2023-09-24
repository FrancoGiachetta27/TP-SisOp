#ifndef SRC_INSTRUCTIONS_EXECUTE_H_
#define SRC_INSTRUCTIONS_EXECUTE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <instructions/decode.h>
#include <pcb/pcb.h>
// #include <commons/string.h>

void execute(t_reg* registers, t_ins complete_instruction, t_log* logger);
uint32_t* select_register(t_reg* registers, char* reg);
void set(t_reg* registers, char* reg, char* value);
void sum(t_reg* registers, char* destination_reg, char* origin_reg);
void sub(t_reg* registers, char* destination_reg, char* origin_reg);

#endif /* SRC_INSTRUCTIONS_EXECUTE_H_ */