#include <user_memory/paging/pages.h>

pthread_mutex_t mtx_select_page;
sem_t sort_pages;
sem_t chage_last_reference;
t_list* pages_to_replace;
t_page* last_page_referenced;
int working;

void send_page_size_to_cpu(t_conn* conn, t_utils* utils) {
    receive_op_code(conn->socket_cpu, utils->logger);
	t_package* package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}

t_page* page_create(int pid, int swap_blocks, int number) {
	t_page* page = malloc(sizeof(*page));

	page->process_pid = pid;
	page->bit_modified = 0;
	page->bit_precense = 0;
	page->frame_number = 0;
	page->page_number = number;
	page->swap_position = swap_blocks;

	return page;
}

t_list* page_table_create(t_pcb* pcb, int swap_blocks, t_log* logger) {
	int total_pages = floor(pcb->tamanio / memory_config.page_size);
	t_list* page_table = list_create();

	for(int i = 0; i < total_pages; i++) {
		list_add(page_table, page_create(pcb->pid, swap_blocks, i));
	}

	log_info(logger, "PID: %d - Tamaño: %d", (int) pcb->pid, total_pages);

	return page_table;
}

t_page* search_page(int pid, int page_number) {
	int _is_pid(t_process* process) {
		return process->pid == pid;
	};
	int _is_page(t_page* page) {
		return page->page_number == page_number;
	};

	t_process* process = list_find(active_processes, (void*) _is_pid);
	t_page* page = list_find(process->page_table, (void*) _is_page);

	sem_wait(&chage_last_reference);
	last_page_referenced = page;
	sem_post(&sort_pages);

	return page;
}

static int is_page_copy(t_page* page) {
	int pid = last_page_referenced->process_pid;
	int frame_number = last_page_referenced->frame_number;

	return page->process_pid == pid && page->frame_number == frame_number;
};

void sort_pages_by_fifo(void) {
    while(working) {
        sem_wait(&sort_pages);

        pthread_mutex_lock(&mtx_select_page);
        if(!list_any_satisfy(pages_to_replace, (void*) is_page_copy)) 
			list_add(pages_to_replace, last_page_referenced);
        pthread_mutex_unlock(&mtx_select_page);

		sem_post(&chage_last_reference);
    }
}

void sort_pages_by_lru(void) {
	int _by_last_reference(t_page* page1, t_page* page2) {
		int64_t temp1 = temporal_gettime(page1->time_reference);
		int64_t temp2 = temporal_gettime(page2->time_reference);

		return temp1 > temp2; 
	};

    while(working) {
        sem_wait(&sort_pages);

		pthread_mutex_lock(&mtx_select_page);
		if(list_any_satisfy(pages_to_replace, (void*) is_page_copy)) {
			list_sort(pages_to_replace, (void*)_by_last_reference);
		}else{
			list_add_in_index(pages_to_replace, 0, last_page_referenced);
		}
		pthread_mutex_lock(&mtx_select_page);

		sem_post(&chage_last_reference);
    }
}

void destroy_page(t_page* page) {
	free(page);
}