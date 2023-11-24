#include <user_memory/paging/pages.h>

t_list *page_tables;

void send_page_size_to_cpu(t_conn *conn, t_utils *utils)
{
	receive_op_code(conn->socket_cpu, utils->logger);
	t_package *package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}

t_page_entry *page_create(uint32_t pid, int bit_modified, int bit_precense, int frame, int page_number, int swap_block)
{
	t_page_entry *page = malloc(sizeof(*page));

	page->pid = pid;
	page->bit_modified = bit_modified;
	page->bit_precense = bit_precense;
	page->frame_number = frame;
	page->page_number = page_number;
	page->swap_position = swap_block;

	return page;
}

void page_table_create(t_pcb *pcb, t_list *swap_blocks, t_log *logger)
{
	int total_pages = floor(pcb->tamanio / memory_config.page_size);
	t_page_table *page_table = malloc(sizeof(*page_table));

	page_table->process_pid = pcb->pid;
	page_table->pages = list_create();

	for (int i = 0; i < total_pages; i++)
	{
		list_add(page_table->pages, page_create(pcb->pid, 0, 0, 0, i, list_get(swap_blocks, i)));
	}

	list_add(page_tables, page_table);

	log_info(logger, "PID: %d - Tamaño: %d", pcb->pid, total_pages);
}

t_page_entry *search_on_table(uint32_t pid, int page_number)
{
	int _is_table(t_page_table * page_table)
	{
		return page_table->process_pid == pid;
	};
	t_page_table *page_table = (t_page_table *)list_find(page_tables, (void *)_is_table);

	return (t_page_entry *)list_get(page_table->pages, page_number);
}

t_page_entry *reference_page(uint32_t pid, int page_number, t_log *logger)
{
	t_page_entry *page = search_on_table(pid, page_number);
	if (page == NULL)
		return NULL;

	if (page->bit_precense == 1)
		log_info(logger, "PID: %d - Pagina: %d - Marco: %d", page->pid, page->page_number, page->frame_number);

	pthread_mutex_lock(&page_reference);
	last_page_referenced = page;
	pthread_mutex_unlock(&page_reference);

	sem_post(&sort_pages);

	return page;
}

void send_page_fault(int socket, t_log *logger)
{
	t_package *package = create_integer_package(PAGE_FAULT_COMMAND, 0);
	send_package(package, socket, logger);
}

void send_page_frame(t_page_entry *page, int socket, t_log *logger)
{
	t_package *package = create_integer_package(PAGE_FRAME, page->frame_number);
	send_package(package, socket, logger);
}

void destroy_page(t_page_entry *page)
{
	free(page);
}

void destroy_page_table(t_page_table *page_table)
{
	list_destroy_and_destroy_elements(page_table->pages, (void *)destroy_page);
	free(page_table);
}