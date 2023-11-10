#ifndef SRC_COMMAND_COMMAND_H_
#define SRC_COMMAND_COMMAND_H_

#define MODULOS_A_CONECTAR 3

#include <pthread.h>
#include <unistd.h>
#include <commons/collections/dictionary.h>
#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <instruction_memory/process/process.h>
#include <instruction_memory/instructions/instructions.h>
#include <user_memory/paging/pages.h>
#include <initial_configuration/memory_config.h>
#include <pcb/pcb.h>

typedef struct {
    t_dictionary* dict;
    t_log* logger;
    int port;
    char* dict_key;
    t_conn* conn;
} t_thread;

enum KERNEL_CODE_OP {
    CREATE_PROCESS = 2,
    PROCESS_OK = 5,
    LOAD_PAGE = 11,
    END_PROCESS = 6,
};

enum CPU_CODE_OP {
    FETCH_INSTRUCTION = 3,
    PAGE_SIZE = 4,
    PAGE_NUMBER = 8,
    PAGE_FRAME = 9,
    PAGE_FAULT = 10,
};

enum FS_OP {
    GET_SWAP_BLOCKS = 7,
};

void wait_in_every_port(t_conn* conn, t_log* logger);
void* wait_for_command(t_thread* thread_info);

#endif /* SRC_COMMAND_COMMAND_H_ */