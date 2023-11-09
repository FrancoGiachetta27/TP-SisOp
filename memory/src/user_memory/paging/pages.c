#include <user_memory/paging/pages.h>

t_list* page_tables; 

void send_page_size_to_cpu(t_conn* conn, t_utils* utils) {
    receive_op_code(conn->socket_cpu, utils->logger);
	t_package* package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}

t_page* page_create(int pid, int swap_blocks, int number) {
	t_page* page = malloc(sizeof(*page));

	page->bit_modified = 0;
	page->bit_precense = 0;
	page->frame_number = 0;
	page->page_number = number;
	page->swap_position = swap_blocks;

	return page;
}

void page_table_create(t_pcb* pcb, int swap_blocks, t_log* logger) {
	int total_pages = floor(pcb->tamanio / memory_config.page_size);
	t_process_pages* page_table = malloc(sizeof(*page_table));

	page_table->process_pid = pcb->pid;
	page_table->pages = list_create();

	for(int i = 0; i < total_pages; i++) {
		list_add(page_table->pages, page_create(pcb->pid, swap_blocks, i));
	}

	list_add(page_tables, page_table);

	log_info(logger, "PID: %d - Tamaño: %d", (int) pcb->pid, total_pages);
}

t_page* search_on_table(int pid, int page_number) {
	int _is_pid(t_process_pages* page_table) {
		return page_table->process_pid == pid;
	};
	int _is_page(t_page* page) {
		return page->page_number == page_number;
	};

	t_process_pages* page_table = (t_process_pages*) list_find(page_tables, (void*) _is_pid);
	
	return (t_page*) list_find(page_table->pages, (void*) _is_page);
}

t_page* reference_page(int pid, int page_number) {
	t_page* page = search_on_table(pid, page_number);

	last_page_referenced = page;
	sem_post(&sort_pages);
	usleep(1000); // just for testing
	page->bit_precense = 1;
	
	return page;
}

void send_page_fault(int socket, t_log* logger) {
	t_package* package = create_empty_package(PAGE_FAULT);
	send_package(package, socket, logger);
}

void send_page_frame(t_page* page, int socket, t_log* logger) {
	t_package* package = create_integer_package(PAGE_FRAME, page->frame_number);
	send_package(package, socket, logger);
}

void destroy_page(t_page* page) {
	free(page);
}