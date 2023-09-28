#ifndef SRC_COMMAND_DISPATCH_H_
#define SRC_COMMAND_DISPATCH_H_

#include <package/package.h>
#include <command/interrupt.h>
#include <instructions/fetch.h>
#include <instructions/decode.h>
#include <instructions/execute.h>
#include <instructions/commands.h>
#include <initial_configuration/server_start.h>

enum DISPATCH_COMMANDS {
   INSTRUCTION = 3
};

int wait_for_dispatch_command(t_utils* utils, t_conn* ports, int memory_socket, t_reg* registers, int page_size);

#endif /* SRC_COMMAND_DISPATCH_H_ */
