#include <stdlib.h>
#include <stdio.h>
#include <initial_configuration/client_start.h>
#include <initial_configuration/server_start.h>
#include <command/command.h>
#include "fcb/fcb.h"
#include "bloque/bloque.h"
#include "fat/fat.h"

#define LOGS_FILE_SYSTEM "filesystem.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	// int memory_socket = connect_to_memory(utils);
	// if (memory_socket == -1) return EXIT_FAILURE;

	// int socket_kernel = start_server_port(utils);
	// if (socket_kernel == -1) {
	// 	close(memory_socket);
	// 	return EXIT_FAILURE;
	// }


	// int wait_for_commands_result = wait_for_commands(socket_kernel, memory_socket, utils);
	
	// utils_destroy_with_connection(utils, memory_socket);
    // return wait_for_commands_result == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
	create_fat_file(utils);
	create_block_file(utils);
	create_fcb_file(utils, "Notas1erParcialK9999");
}
