#include <user_memory/paging/algorithms.h>

pthread_mutex_t mtx_select_page;
sem_t sort_pages;
t_list* pages_to_replace;
int working = 1;

static int is_page_copy(t_page* page) {
	int same_page_number = page->page_number == last_page_referenced->page_number;
	int same_pid = page->pid == last_page_referenced->page_number;
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
		if(last_page_referenced->bit_precense == 1) {
			t_page* lr_page = (t_page*) list_remove_by_condition(pages_to_replace, (void*) is_page_copy);
			list_add(pages_to_replace, lr_page);
			pthread_mutex_unlock(&mtx_select_page);
		}else{
			list_add(pages_to_replace, last_page_referenced);
			pthread_mutex_unlock(&mtx_select_page);
		}
    }
}