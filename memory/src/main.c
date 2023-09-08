#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <initial_configuration/server_start.h>
#include <utils/utils.h>
#include <package/package.h>

#define LOGS_MEMORIA "memoria.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	t_conn* conn = start_server_ports(utils);
	if (conn == NULL) return EXIT_FAILURE;

	int op_code_pkg = receive_op_code(conn->socket_filesystem, utils->logger);

	char* message;
	void* buffer;

	switch(op_code_pkg) {
		case KERNEL_MEMORY:
			buffer = receive_package(conn->socket_kernel);
			message = deserialize_buffer(buffer);
			log_info(utils->logger, "%s", message);
			free(message);
			break;
		case FILESYSTEM_MEMORY:
			buffer = receive_package(conn->socket_filesystem);
			message = deserialize_buffer(buffer);
			log_info(utils->logger, "%s", message);
			free(message);
			break;
		default:
			log_error(utils->logger, "Error codigo incorrecto");
	}

	log_trace(utils->logger, "Se termina el programa");
	free(conn);
	utils_destroy(utils);
    return 0;
}
