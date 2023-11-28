#ifndef SRC_USER_MEMORY_PAGING_ALGORITHMS_H
#define SRC_USER_MEMORY_PAGING_ALGORITHMS_H

#include <initial_configuration/memory_config.h>
#include <commons/collections/list.h>
#include <semaphore.h>
#include <user_memory/paging/pages.h>

pthread_mutex_t mtx_select_page;
extern int working;

void remove_from_victims(t_page_entry* victim);
void sort_pages_by_fifo(void);
void sort_pages_by_lru(void);
void init_sorter_thread(void); 

#endif /* SRC_USER_MEMORY_PAGING_ALGORITHMS_H */