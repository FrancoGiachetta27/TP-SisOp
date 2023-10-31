#include <user_memory/paging/pages.h>

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

	last_page_referenced = page;
	sem_post(&sort_pages);
	
	return page;
}

void destroy_page(t_page* page) {
	free(page);
}