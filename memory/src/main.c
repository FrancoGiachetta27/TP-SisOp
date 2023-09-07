#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <initial_configuration/server_start.h>

#define LOGS_MEMORIA "memoria.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	t_conn* conn = start_server_ports(utils);
	if (conn == NULL) return EXIT_FAILURE;

	log_trace(utils->logger, "Se termina el programa");
	free(conn);
	utils_destroy(utils);
    return 0;
}
