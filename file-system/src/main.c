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

extern t_list *fat_list;

int main(int argc, char *argv[])
{
	utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL)
		return EXIT_FAILURE;

	init_fs_config(utils->config, &fs_config);

	// fcbs = list_create();

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

	// // FAT
	// create_fat_file();
	// initialize_fat_list();

	// assign_block_size(20);
	// assign_block_size(5);
	// assign_block_size(100);
	// print_fat();

	// Bloques

	// SWAP
	// create_block_file();
	// t_list *a = reserve_swap_blocks(5);
	// for (int i = 0; i < list_size(a); i++)
	// {
	// 	log_debug(utils->logger, "Reservar devuelve bloque: %d", list_get(a, i));
	// }
	// t_list *b = list_create();
	// list_add(b, 0);
	// list_add(b, 2);
	// free_swap_blocks(b);
	// t_list *c = reserve_swap_blocks(10);
	// print_swap();

	// list_destroy(a);
	// list_destroy(c);
	// destroy_swap_list();
	// destroy_fs_config(&fs_config);

	create_block_file();
	t_list *reserved_blocks = reserve_swap_blocks(3);

	// Imprime los bloques reservados
	for (int i = 0; i < list_size(reserved_blocks); i++)
	{
		int block_index = (int)list_get(reserved_blocks, i);
		printf("Bloque reservado: %d\n", block_index);
	}

	// Libera algunos bloques
	// t_list *b = list_create();
	// list_add(b, 1);
	// free_swap_blocks(b);

	// list_destroy(reserved_blocks);

	// Datos para escribir en un bloque de la swap
	void *data = "Q ONDA 213";

	write_to_swap_block(2, data);

	void *buffer = read_from_swap_block(2);
	printf("Buffer: %s\n", (char *)buffer);

	free(buffer);
	list_destroy(reserved_blocks);

	// create_fat_file(utils);
	// create_block_file(utils);

	// create_file(utils, "Notas1erParcialK9999");
	// create_file(utils, "Testing");
	// create_file(utils, "Damian");

	// truncate_file(utils, "Damian", 30);
	// truncate_file(utils, "Damian", 100);
	// truncate_file(utils, "Testing", 10);
	// truncate_file(utils, "Damian", 3);
	// truncate_file(utils, "Notas1erParcialK9999", 40);

	// print_fat();

	// print_fcb_list();
	// print_fat();
}
