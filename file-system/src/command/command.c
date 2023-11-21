#include <command/command.h>
#include <fcntl.h>
#include <fcb/fcb.h>
#include <fs-struct/fat-bloque.h>
#include "initial_configuration/fs_config.h"
#include "page/page.h"

// extern t_utils *utils;

int wait_for_commands(int socket_kernel, int memory_socket, t_utils *utils)
{
	int op_code = receive_op_code(socket_kernel, utils->logger);
	if (op_code == -1)
	{
		return -1;
	}
	while (op_code)
	{
		char *message;
		char *file_name;
		int block_count;
		int block;
		t_pag_swap *page_swap;
		t_package *package;

		switch (op_code)
		{
		case ECHO_FILESYSTEM:
			message = receive_buffer(socket_kernel, utils->logger);
			log_info(utils->logger, "OpCode: %d and Message: %s", op_code, message);
			package = create_string_package(ECHO_MEMORY, "ECHO To Memory From FS");
			send_package(package, memory_socket, utils->logger);
			free(message);
			break;

		case F_OPEN:
			file_name = receive_buffer(socket_kernel, utils->logger);
			log_info(utils->logger, "F_OPEN Kernel con archivo %s", file_name);
			int file_size = open_file(utils, file_name);
			package = create_integer_package(F_OPEN, file_size);
			log_info(utils->logger, "Se envio el file size %d a Kernel", file_size);
			send_package(package, socket_kernel, utils->logger);
			free(file_name);
			break;

		case F_CREATE:
			file_name = receive_buffer(socket_kernel, utils->logger);
			log_info(utils->logger, "F_CREATE Kernel con archivo %s", file_name);
			int ok = create_file(utils, file_name);
			package = create_string_package(F_CREATE, "OK");
			log_info(utils->logger, "Se envio el OK a Kernel");
			send_package(package, socket_kernel, utils->logger);
			free(file_name);
			break;

		case F_TRUNCATE:
			file_name = "Damian"; // Receive buffer
			file_size = 100;	  // Receive buffer
			log_info(utils->logger, "F_TRUNCATE Kernel con archivo %s y tamaño %d", file_name, file_size);
			truncate_file(utils->logger, file_name, file_size);
			package = create_string_package(F_TRUNCATE, "OK");
			log_info(utils->logger, "Se trunco el archivo y devuelvo OK al Kernel");
			send_package(package, socket_kernel, utils->logger);
			// free(file_name);
			break;

		// SWAP
		// Recibo cantidad de bloques (int) - reservo - devuelvo lista?
		case GET_SWAP_BLOCKS:
			block_count = receive_buffer(memory_socket, utils);
			log_info(utils->logger, "GET_SWAP_BLOCKS Memoria necesita %d de bloques SWAP reservados", block_count);
			t_list *blocks_reserved = reserve_swap_blocks(block_count);
			void *buffer = serialize_list(blocks_reserved);
			// Enviar paquete array
			send_list(GET_SWAP_BLOCKS, blocks_reserved, memory_socket, utils->logger);
			list_destroy(blocks_reserved);
			free(block_count);
			break;

		// Recibo bloque a leer (int) - leo data - devuelvo solo el void * con la informacion?
		case GET_FROM_SWAP:
			block = receive_buffer(memory_socket, utils);
			log_info(utils->logger, "Acceso a Bloque SWAP: “Acceso SWAP: %d”", block);
			void *data = read_from_swap_block(block);
			package = create_string_package(GET_FROM_SWAP, (char *)data);
			send_package(package, memory_socket, utils->logger);
			free(block);
			break;

		// Recibo t_pag_swap - devuelvo un ok?
		case UPDATE_SWAP:
			page_swap = receive_page_for_swap(memory_socket, utils->logger);
			log_info(utils->logger, "Actualizando Bloque SWAP: “Acceso SWAP: %d”", block);
			write_to_swap_block(page_swap->swap_block, page_swap->page_content);
			// QUE ENVIAR? YA QUE SOLO ACTUALIZO EN FILESYSTEM
			package = create_string_package(UPDATE_SWAP, "OK");
			send_package(package, memory_socket, utils->logger);
			break;

		// Recibo una lista o que de bloques a liberar - libero - devuelvo ok?
		case FREE_PAGES:
			break;

		default:
			log_error(utils->logger, "Unknown OpCode");
			utils_destroy_with_connection(utils, memory_socket);
			return -1;
		}
		int op_code = receive_op_code(socket_kernel, utils->logger);
		if (op_code == -1)
			return -1;
	}
	return 0;
}

// Return file size si existe
// Return informar que el archivo no existe
int open_file(t_utils *utils, char *file_name)
{
	log_info(utils->logger, "Abrir Archivo: %s", file_name);
	t_fcb *file = find_fcb_file(file_name);
	if (file)
	{
		log_info(utils->logger, "Archivo %s abierto", file_name);
		return file->file_size;
	}

	log_info(utils->logger, "Archivo %s no existe", file_name);
	return 0;
}

// Return OK
int create_file(t_utils *utils, char *file_name)
{
	log_info(utils->logger, "Crear Archivo: %s", file_name);
	create_fcb_file(file_name);
	return 1;
}

void truncate_file(t_utils *utils, char *file_name, int new_size)
{
	log_info(utils->logger, "Truncar Archivo: %s - Tamaño: %d", file_name, new_size);

	t_fcb *fcb = find_fcb_file(file_name);

	// Condicion de incremento, igualdad o decremento del nuevo tamanio

	if (fcb->file_size == new_size)
	{
		log_info(utils->logger, "El nuevo tamaño es el mismo que antes, no se trunco.");
		return /* Ver que devolver*/;
	}

	// Caso que el file tiene tamanio 0 y asigno el initial block y el tamanio
	if (fcb->file_size == 0)
	{
		int first_block = assign_block_size(new_size);
		fcb->initial_block = first_block;
	}
	// Tamanio nuevo mayor que al del archivo
	else if (fcb->file_size < new_size)
	{
		int current_size = fcb->file_size / sizeof(uint32_t) + 1;
		int new_block_count = (new_size / sizeof(uint32_t)) + 1;

		log_debug(utils->logger, "Cantidad de bloques actuales: %d - Cantidad de bloques total nuevos %d", current_size, new_block_count);

		int blocks_to_add = new_block_count - current_size - 1;

		log_debug(utils->logger, "Cantidad de bloques que hay que agregar: %d", blocks_to_add);

		add_blocks(fcb->initial_block, blocks_to_add);
	}
	else
	{
		int current_size = fcb->file_size / sizeof(uint32_t) + 1;
		int blocks_needed = (new_size / sizeof(uint32_t)) + 1;

		log_debug(utils->logger, "Cantidad de bloques actuales: %d - Cantidad de bloques nuevos %d", current_size, blocks_needed);

		free_blocks(fcb->initial_block, blocks_needed);
	}

	fcb->file_size = new_size;

	// Actualizar file FCB
	update_fcb(fcb);
}