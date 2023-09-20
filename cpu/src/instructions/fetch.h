#ifndef SRC_INSTRUCTIONS_FETCH_H_
#define SRC_INSTRUCTIONS_FETCH_H_

#include <package/package.h>
#define FETCH_INSTRUCTION 3

char* fetch(int* program_counter, int pid, int memory_socket, t_log* logger);
void* serialize_fetch(int program_counter, int pid);

#endif /* SRC_INSTRUCTIONS_FETCH_H_ */