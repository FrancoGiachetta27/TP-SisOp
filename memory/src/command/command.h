#ifndef SRC_COMMAND_COMMAND_H_
#define SRC_COMMAND_COMMAND_H_

#define MODULOS_A_CONECTAR 3

#include <pthread.h>
#include <commons/collections/dictionary.h>
#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <instruction_memory/process/process.h>
#include <instruction_memory/instructions/instructions.h>

typedef struct {
    t_dictionary* dict;
    t_log* logger;
    int port;
    char* dict_key;
} t_thread;

enum MEMORY_CODE_OP {
    CREATE_PROCESS = 2,
    FETCH_INSTRUCTION = 3,
    PAGE_SIZE = 4,
};

void wait_in_every_port(t_conn* conn, t_log* logger);
void* wait_for_command(t_thread* thread_info);

#endif /* SRC_COMMAND_COMMAND_H_ */