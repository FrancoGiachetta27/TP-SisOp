#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <commons/string.h>
#include <config/config.h>
//#include <connection/connection.h>
//#include <handshake/handshake.h>
#include <package/package.h>
#include <package_fs/package_fs.h>
#include <initial_configuration/client_start.h>
#include <initial_configuration/server_start.h>

#define LOGS_FILE_SYSTEM "filesystem.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	int memory_socket = connect_to_memory(utils);
	if (memory_socket == -1) return EXIT_FAILURE;

	int server_fd = start_server_port(utils);
	if (server_fd == -1) {
		close(memory_socket);
		return EXIT_FAILURE;
	}
	log_info(utils->logger, "%d", server_fd);
	int op_code_pkg = receive_op_code(server_fd, utils->logger);
	log_info(utils->logger, "%d", op_code_pkg);
	switch(op_code_pkg) {
		case FILESYSTEM_MEMORY:
			void* buffer = receive_package(memory_socket);
			redirect_package(buffer, memory_socket, FILESYSTEM_MEMORY);
			log_info(utils->logger, "Redireccionado mensaje...");
			break;
		default:
			log_error(utils->logger, "Error codigo incorrecto");
		}

	utils_destroy_with_connection(utils, memory_socket);
    return 0;
}
