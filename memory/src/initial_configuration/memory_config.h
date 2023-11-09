#ifndef SRC_INITIAL_CONFIGURATION_MEMORY_CONFIG_H_
#define SRC_INITIAL_CONFIGURATION_MEMORY_CONFIG_H_

#include <config/config.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <instruction_memory/process/process.h>
#include <user_memory/paging/algorithms.h>
#include <user_memory/paging/pages.h>
#include <user_memory/real_memory.h>
#include <semaphore.h>

typedef struct {
    int memory_size;
    int page_size;
    char* instructions_path;
    char* algorithm;
    int time_delay;
} t_memory_cfg;

void init_memory_config(t_config* config);
void init_real_memory(void);
void free_memory(void);

extern t_memory_cfg memory_config;

#endif /*SRC_INITIANL_CONFIGURATION_MEMORY_CONFIG_H_*/