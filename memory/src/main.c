#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <command/command.h>
#include <initial_configuration/memory_config.h>
#include <instruction_memory/process/process.h>

#define LOGS_MEMORIA "memoria.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;
	
	init_memory(utils->config, &memory_config, &active_processes);

	t_conn* conn = start_server_ports(utils);
	if (conn == NULL) return EXIT_FAILURE;
	
	int is_ok = send_page_size_to_cpu(conn, utils);

	if(is_ok != 1) {
		free(conn);
		utils_destroy(utils);
		return EXIT_FAILURE;
	} 

	wait_in_every_port(conn, utils->logger);

	log_trace(utils->logger, "Se termina el programa");
	free(conn);
	utils_destroy(utils);
    return EXIT_SUCCESS;
}