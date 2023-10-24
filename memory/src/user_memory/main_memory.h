#ifndef SRC_USER_MEMORY_MAIN_MEMORY_H
#define SRC_USER_MEMORY_MAIN_MEMORY_H

#include <stdlib.h>
#include <initial_configuration/memory_config.h>
#include <commons/bitarray.h>

typedef struct {
    void* frames;
    t_bitarray* frame_table;
} user_space;

extern user_space main_memory;

void init_main_memory(void);
int check_available_frames(void);
int* get_swap_blocks(int bytes,int socket_fs, t_log* logger);
void read_frame(void);
void write_on_frame(void);

#endif /* SRC_USER_MEMORY_MAIN_MEMORY_H */