#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <initial_configuration/client_start.h>

#define LOGS_FILE_SYSTEM "filesystem.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	int memory_socket = connect_to_memory(utils);
	if (memory_socket == -1) return EXIT_FAILURE;

	if (!check_if_config_value_exists(utils, "PUERTO_ESCUCHA")) {
		utils_destroy_with_connection(utils, memory_socket);
		return EXIT_FAILURE;
	}
	int server_fd = start_server(config_get_string_value(utils->config, "PUERTO_ESCUCHA"), utils->logger);
	if (server_fd == -1) {
		utils_destroy_with_connection(utils, memory_socket);
		return EXIT_FAILURE;
	}
	log_trace(utils->logger, "Se inicio correctamente el server");
	int initial_handshake_result = wait_for_initial_handshake_from_kernel(server_fd, utils->logger);
	if (initial_handshake_result == -1) {
		utils_destroy_with_connection(utils, memory_socket);
		return EXIT_FAILURE;
	}

	utils_destroy_with_connection(utils, memory_socket);
    return 0;
}
