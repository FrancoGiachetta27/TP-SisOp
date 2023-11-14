#ifndef SRC_COMMAND_COMMAND_H_
#define SRC_COMMAND_COMMAND_H_

#include <package/package.h>
#include <config/config.h>

// F_OPEN,
// F_CREATE,
// F_TRUNCATE,

enum KERNEL_CODE_OP
{
    F_OPEN = 19,
    F_CREATE = 20,
    F_TRUNCATE = 21
};

int wait_for_commands(int socket_kernel, int memory_socket, t_utils *utils);
int open_file(t_utils *utils, char *file_name);
int create_file(t_utils *utils, char *file_name);
void truncate_file(t_utils *utils, char *file_name, int new_size);

#endif /* SRC_COMMAND_COMMAND_H_ */