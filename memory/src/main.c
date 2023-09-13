#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <command/command.h>
#include <initial_configuration/memory_config.h>
#include <instruction_memory/instruction_memory.h>

#define LOGS_MEMORIA "memoria.log"

t_memory_cfg memory_config;

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	init_memory(utils, &memory_config);

	t_conn* conn = start_server_ports(utils);
	if (conn == NULL) return EXIT_FAILURE;

	wait_in_every_port(conn, utils->logger);

	log_trace(utils->logger, "Se termina el programa");
	free(conn);
	utils_destroy(utils);
    return EXIT_SUCCESS;
}
