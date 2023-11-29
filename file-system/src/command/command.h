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
    F_TRUNCATE = 21,
    F_READ = 22
};

enum MEMORY_CODE_OP
{
    GET_SWAP_BLOCKS = 7,
    GET_FROM_SWAP = 12,
    UPDATE_SWAP = 13,
    FREE_PAGES = 14,
};

typedef struct
{
    t_dictionary *dict;
    t_log *logger;
    int port;
    char *dict_key;
    int socket;
} t_thread;

void *wait_for_commands(t_thread *);
int open_file(t_log *logger, char *file_name);
int create_file(t_log *logger, char *file_name);
void truncate_file(t_log *logger, char *file_name, int new_size);
void wait_in_every_port(int memory, int kernel, t_log *logger);

#endif /* SRC_COMMAND_COMMAND_H_ */