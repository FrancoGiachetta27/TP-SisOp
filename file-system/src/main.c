#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <package/package.h>
#include <initial_configuration/client_start.h>
#include <initial_configuration/server_start.h>

#define LOGS_FILE_SYSTEM "filesystem.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	int memory_socket = connect_to_memory(utils);
	if (memory_socket == -1) return EXIT_FAILURE;

	int socket_kernel = start_server_port(utils);
	if (socket_kernel == -1) {
		close(memory_socket);
		return EXIT_FAILURE;
	}

	int op_code = receive_op_code(socket_kernel, utils->logger);
	if (op_code == -1) {
		utils_destroy_with_connection(utils, memory_socket);
		return EXIT_FAILURE;
	}
	while (op_code)
	{
		switch (op_code) {
			case ECHO_FILESYSTEM:
				char* message = receive_buffer(socket_kernel, utils->logger);
				log_info(utils->logger, "OpCode: %d and Message: %s", op_code, message);
				free(message);
				break;
			default:
				log_error(utils->logger, "Unknown OpCode");
				utils_destroy_with_connection(utils, memory_socket);
				return EXIT_FAILURE;
		}
		int op_code = receive_op_code(socket_kernel, utils->logger);
		if (op_code == -1) {
			utils_destroy_with_connection(utils, memory_socket);
			return EXIT_FAILURE;
		}
	}
	
	utils_destroy_with_connection(utils, memory_socket);
    return 0;
}
