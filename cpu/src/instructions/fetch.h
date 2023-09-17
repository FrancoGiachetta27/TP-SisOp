#ifndef SRC_INSTRUCTIONS_FETCH_H_
#define SRC_INSTRUCTIONS_FETCH_H_

#include <package/package.h>

char* fetch(int* program_counter, int memory_socket, t_log* logger);

#endif /* SRC_INSTRUCTIONS_FETCH_H_ */