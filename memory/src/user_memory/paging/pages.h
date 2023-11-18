#ifndef SRC_USER_MEMORY_PAGING_PAGES_H
#define SRC_USER_MEMORY_PAGING_PAGES_H

#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/memory_config.h>
#include <user_memory/paging/algorithms.h>
#include <command/opcodes.h>
#include <pcb/pcb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <semaphore.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    uint32_t pid;
    int page_number;
    int frame_number;
    short int bit_precense;
    short int bit_modified;
    int swap_position;
} t_page_entry;

typedef struct {
    uint32_t process_pid;
    t_list* pages;
} t_page_table;

extern t_list* page_tables;
t_page_entry* last_page_referenced;

void send_page_size_to_cpu(t_conn* conn, t_utils* utils);
t_page_entry* page_create(uint32_t pid, int bit_modified, int bit_precense, int frame, int page_number, int swap_block);
void page_table_create(t_pcb* process, int swap_blocks, t_log* logger);
t_page_entry* reference_page(uint32_t pid, int page_number, t_log* logger);
void send_page_fault(int socket, t_log* logger);
void send_page_frame(t_page_entry* page, int socket, t_log* logger);
void destroy_page(t_page_entry* page);
void destroy_page_table(t_page_table* page_table);

#endif /* SRC_USER_MEMORY_PAGING_PAGES_H */