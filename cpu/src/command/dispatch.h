#ifndef SRC_COMMAND_DISPATCH_H_
#define SRC_COMMAND_DISPATCH_H_

#include <package/package.h>
#include <command/interrupt.h>
#include <initial_configuration/server_start.h>

int wait_for_dispatch_command(t_utils* utils, t_conn* conn, int memory_socket);

#endif /* SRC_COMMAND_DISPATCH_H_ */
