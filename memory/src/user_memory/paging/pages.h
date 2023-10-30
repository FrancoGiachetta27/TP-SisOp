#ifndef SRC_USER_MEMORY_PAGING_PAGES_H
#define SRC_USER_MEMORY_PAGING_PAGES_H

#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/memory_config.h>
#include <instruction_memory/process/process.h>
#include <pcb/pcb.h>
#include <command/command.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>

typedef struct {
    int process_pid;
    int page_number;
    int frame_number;
    short int bit_precense;
    short int bit_modified;
    t_temporal* time_reference;
    int swap_position;
} t_page;

pthread_mutex_t mtx_select_page;
sem_t sort_pages;
sem_t chage_last_reference;
extern t_list* pages_to_replace;
t_page* last_page_referenced;
int working;

t_page* page_create(int pid, int swap_blocks, int number);
t_list* page_table_create(t_pcb* process, int swap_blocks, t_log* logger);
t_page* search_page(int pid, int page_number);
void sort_pages_by_fifo(void);
void sort_pages_by_lru(void);
void send_page_size_to_cpu(t_conn* conn, t_utils* utils);
void destroy_page(t_page* page);

#endif /* SRC_USER_MEMORY_PAGING_PAGES_H */