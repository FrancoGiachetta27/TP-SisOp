#include <user_memory/paging/pages.h>

void send_page_size_to_cpu(t_conn* conn, t_utils* utils) {
    receive_op_code(conn->socket_cpu, utils->logger);
	t_package* package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}

t_list* page_table_create(int process_bytes, int swap_blocks) {
	int total_pages = process_bytes / memory_config.page_size;
	t_list* page_table = list_create();

	for(int i = 0; i < total_pages; i++) {
		t_page* page = malloc(sizeof(*page));

		int frame_number = check_available_frames();
		
		page->bit_modified = 0;
		page->bit_precense = 0;
		page->reference_instant = NULL;
		page->frame_number = frame_number;
		page->swap_position = swap_blocks;

		list_add(page_table, page);
	}

	return page_table;
}

