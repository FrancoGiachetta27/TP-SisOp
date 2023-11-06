#include <stdlib.h>
#include <stdio.h>
#include <initial_configuration/client_start.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/fs_config.h>
#include <command/command.h>
#include "fcb/fcb.h"
#include "fs-struct/fat-bloque.h"
#include <commons/bitarray.h>
#include <commons/memory.h>

#define LOGS_FILE_SYSTEM "filesystem.log"

t_utils *utils;
t_list *fcbs;

int main(int argc, char *argv[])
{
	utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL)
		return EXIT_FAILURE;
	init_fs_config(utils->config, &fs_config);

	fcbs = list_create();

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


// TESTING

	create_fat_file();
	initialize_fat_list();


	assign_block_size(10);
	// assign_block_size(7);
	print_fat();

	// create_fat_file(utils);
	// create_block_file(utils);

	// create_file(utils, "Notas1erParcialK9999");
	// create_file(utils, "Testing");
	// create_file(utils, "Damian");

	// open_file(utils, "Damian");
	// open_file(utils, "NoExisto");

	// t_fcb *fcb = find_fcb_file("Testing");

	// truncate_file(utils, "Damian", 100);
	// print_fcb_list();
}
