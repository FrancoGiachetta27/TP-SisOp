#ifndef SRC_USER_MEMORY_REAL_MEMORY_H
#define SRC_USER_MEMORY_REAL_MEMORY_H

#include <stdlib.h>
#include <initial_configuration/memory_config.h>
#include <user_memory/paging/pages.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <pthread.h>

typedef struct {
    void* frames;
    t_bitarray* frame_table;
} t_user_space;

typedef struct {
    bool available;
    int frame_number;
} t_frame_search;

pthread_mutex_t mtx_frame_access;
extern t_user_space real_memory;

t_frame_search check_available_frames(void);

int get_swap_blocks(int bytes, int socket_fs, t_log* logger);
uint32_t read_frame(int pid, int real_address, t_log* logger);
void/*uint32_t*/ write_on_frame(int pid, int real_address, t_log* logger, uint32_t data);
void load_page(int pid, int page_number, t_log* logger);

#endif /* SRC_USER_MEMORY_REAL_MEMORY_H */