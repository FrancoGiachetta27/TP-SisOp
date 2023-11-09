#ifndef SRC_USER_MEMORY_PAGING_PAGES_H
#define SRC_USER_MEMORY_PAGING_PAGES_H

#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/memory_config.h>
#include <instruction_memory/process/process.h>
#include <user_memory/paging/algorithms.h>
#include <pcb/pcb.h>
#include <command/command.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/collections/list.h>
#include <semaphore.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int page_number;
    int frame_number;
    short int bit_precense;
    short int bit_modified;
    int swap_position;
} t_page;

typedef struct {
    int process_pid;
    t_list* pages;
} t_process_pages;

extern t_list* page_tables;
t_page* last_page_referenced;

void send_page_size_to_cpu(t_conn* conn, t_utils* utils);
t_page* page_create(int pid, int swap_blocks, int number);
void page_table_create(t_pcb* process, int swap_blocks, t_log* logger);
t_page* search_on_table(int pid, int page_number);
t_page* reference_page(int pid, int page_number, t_log* logger);
void send_page_fault(int socket, t_log* logger);
void send_page_frame(t_page* page, int socket, t_log* logger);
void destroy_page(t_page* page);

#endif /* SRC_USER_MEMORY_PAGING_PAGES_H */