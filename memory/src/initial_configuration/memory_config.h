#ifndef SRC_INITIAL_CONFIGURATION_MEMORY_CONFIG_H_
#define SRC_INITIAL_CONFIGURATION_MEMORY_CONFIG_H_

#include <config/config.h>
#include <commons/config.h>
#include <instruction_memory/process/process.h>
#include <user_space/main_memory.h>
#include <commons/bitarray.h>

typedef struct {
    int memory_size;
    int page_size;
    char* instructions_path;
    char* algorithm;
    int time_delay;
} t_memory_cfg;

void init_memory(t_config* config, t_memory_cfg* memory_config, t_list** active_processes);

extern t_memory_cfg memory_config;

#endif /*SRC_INITIANL_CONFIGURATION_MEMORY_CONFIG_H_*/