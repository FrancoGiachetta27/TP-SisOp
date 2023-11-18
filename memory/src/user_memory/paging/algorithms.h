#ifndef SRC_USER_MEMORY_PAGING_ALGORITHMS_H
#define SRC_USER_MEMORY_PAGING_ALGORITHMS_H

#include <initial_configuration/memory_config.h>
#include <commons/collections/list.h>
#include <semaphore.h>

pthread_mutex_t mtx_select_page;
sem_t sort_pages;
extern t_list* pages_to_replace;
int working;

void sort_pages_by_fifo(void);
void sort_pages_by_lru(void);
void init_sorter_thread(void); 

#endif /* SRC_USER_MEMORY_PAGING_ALGORITHMS_H */