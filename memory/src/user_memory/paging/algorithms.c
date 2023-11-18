#include <user_memory/paging/algorithms.h>

pthread_mutex_t mtx_select_page;
pthread_mutex_t page_reference;
sem_t sort_pages;
t_list* pages_to_replace;
int working = 1;

static bool is_page_copy(t_page_entry* page) {
	int hola = list_size(pages_to_replace);
	pthread_mutex_lock(&page_reference);
	bool same_page_number = page->page_number == last_page_referenced->page_number;
	bool same_pid = page->pid == last_page_referenced->pid;
	pthread_mutex_unlock(&page_reference);
	return same_pid && same_page_number;
};

void sort_pages_by_fifo(void) {
    while(working) {
        sem_wait(&sort_pages);
        pthread_mutex_lock(&mtx_select_page);
		if(last_page_referenced->bit_precense != 1) {
			list_add(pages_to_replace, last_page_referenced);
		}
		pthread_mutex_unlock(&mtx_select_page);
   	}
}

void sort_pages_by_lru(void) {
    while(working) {
        sem_wait(&sort_pages);
		pthread_mutex_lock(&mtx_select_page);
		if(!list_is_empty(pages_to_replace) && last_page_referenced->bit_precense == 1) {
			list_remove_and_destroy_by_condition(pages_to_replace, (void*) is_page_copy, (void*) destroy_page);
			list_add(pages_to_replace, last_page_referenced);
			pthread_mutex_unlock(&mtx_select_page);
		}else{
			list_add(pages_to_replace, last_page_referenced);
			pthread_mutex_unlock(&mtx_select_page);
		}
    }
}

void init_sorter_thread(void) {
    pthread_t page_replace_sorter_theard;
    pages_to_replace = list_create();
    sem_init(&sort_pages, 0, 0);
    
    if(string_equals_ignore_case(memory_config.algorithm, "FIFO")){
        pthread_create(&page_replace_sorter_theard, NULL, (void*) sort_pages_by_fifo, NULL);
    }else {
        pthread_create(&page_replace_sorter_theard, NULL, (void*) sort_pages_by_lru, NULL);
    }
    
    pthread_detach(page_replace_sorter_theard);
}