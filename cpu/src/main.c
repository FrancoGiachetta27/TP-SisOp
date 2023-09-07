#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <initial_configuration/client_start.h>
#include <initial_configuration/server_start.h>

#define LOGS_CPU "cpu.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_CPU, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	int memory_socket = connect_to_memory(utils);
	if (memory_socket == -1) return EXIT_FAILURE;

	t_conn* ports = start_server_ports(utils);
	if (ports == NULL) {
		close(memory_socket);
		return EXIT_FAILURE;
	}

	utils_destroy_with_connection(utils, memory_socket);
	free(ports);
    return 0;
}
