#ifndef SRC_USER_MEMORY_REAL_MEMORY_H
#define SRC_USER_MEMORY_REAL_MEMORY_H

#include <stdlib.h>
#include <initial_configuration/memory_config.h>
#include <math.h>
#include <commons/bitarray.h>
#include <pthread.h>
#include <ctype.h>

typedef struct {
    void* frames;
    t_bitarray* frame_table;
} user_space;

pthread_mutex_t mtx_frame_access;
extern user_space real_memory;

int check_available_frames(void);

int get_swap_blocks(int bytes,int socket_fs, t_log* logger);
int read_frame(int real_address);
void write_on_frame(int real_address, int data);
void load_page(int log_address);
void search_page(void);

#endif /* SRC_USER_MEMORY_REAL_MEMORY_H */