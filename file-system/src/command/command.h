#ifndef SRC_COMMAND_COMMAND_H_
#define SRC_COMMAND_COMMAND_H_

#include <package/package.h>
#include <config/config.h>

int wait_for_commands(int socket_kernel, int memory_socket, t_utils* utils);

#endif /* SRC_COMMAND_COMMAND_H_ */