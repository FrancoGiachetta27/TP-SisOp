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
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int process_pid;
    int page_number;
    int frame_number;
    short int bit_precense;
    short int bit_modified;
    int swap_position;
} t_page;

t_page* last_page_referenced;

void send_page_size_to_cpu(t_conn* conn, t_utils* utils);
t_page* page_create(int pid, int swap_blocks, int number);
t_list* page_table_create(t_pcb* process, int swap_blocks, t_log* logger);
t_page* search_page(int pid, int page_number);
void send_page_fault(int socket, t_log* logger) 
void send_page_frame(t_page* page, int socket, t_log* logger)
void destroy_page(t_page* page);

#endif /* SRC_USER_MEMORY_PAGING_PAGES_H */