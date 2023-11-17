#ifndef SRC_USER_MEMORY_REAL_MEMORY_H
#define SRC_USER_MEMORY_REAL_MEMORY_H

#include <stdlib.h>
#include <package/package.h>
#include <initial_configuration/memory_config.h>
#include <user_memory/paging/pages.h>
#include <page/page.h>
#include <command/command.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <commons/log.h>
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
void* read_frame(int real_address, size_t size);
void write_on_frame(int real_address, size_t size, void* data);
void load_page(int pid, int page_number, int fs_socket,t_log* logger);

#endif /* SRC_USER_MEMORY_REAL_MEMORY_H */