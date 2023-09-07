#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
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

	utils_destroy_with_connection(utils, memory_socket);
    return 0;
}
