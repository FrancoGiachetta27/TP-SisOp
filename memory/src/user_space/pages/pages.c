#include <user_space/pages/pages.h>

void send_page_size_to_cpu(t_conn* conn, t_utils* utils) {
    receive_op_code(conn->socket_cpu, utils->logger);
	t_package* package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}

t_list* page_table_create(process_bytes) {
	int total_pages = process_bytes / memory_config.page_size;
	t_list* page_table = list_create();

	for(int i = 0; i < total_pages; i++) {
		t_page* page;
		
		page->bit_modified = 0;
		page->bit_precense = 0;

		if(string_equals_ignore_case(memory_config.algorithm, "LRU")) page->reference_instant = temporal_create();

		int frame = check_available_frames();

		list_add(page_table, page);
	}
}