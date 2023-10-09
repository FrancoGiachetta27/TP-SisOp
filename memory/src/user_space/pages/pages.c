#include <user_space/pages/pages.h>

int send_page_size_to_cpu(t_conn* conn, t_utils* utils) {
    receive_op_code(conn->socket_cpu, utils->logger);
	t_package* package_page = create_integer_package(PAGE_SIZE, memory_config.page_size);
	send_package(package_page, conn->socket_cpu, utils->logger);
}