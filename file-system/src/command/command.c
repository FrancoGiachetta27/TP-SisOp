#include <command/command.h>
#include <fcntl.h>
#include <fcb/fcb.h>
#include <fs-struct/fat-bloque.h>
#include "initial_configuration/fs_config.h"
#include "page/page.h"

// extern t_utils *utils;

void *wait_for_commands(t_thread *thread_info)
{
	int op_code = receive_op_code(thread_info->port, thread_info->logger);
	if (op_code == -1)
	{
		return -1;
	}
	while (op_code)
	{
		char *message;
		char *file_name;
		int *block;
		t_pag_swap *page_swap;
		t_package *package;

		switch (op_code)
		{
		case ECHO_FILESYSTEM:
			message = receive_buffer(thread_info->port, thread_info->logger);
			log_info(thread_info->logger, "OpCode: %d and Message: %s", op_code, message);
			package = create_string_package(ECHO_MEMORY, "ECHO To Memory From FS");
			send_package(package, thread_info->port, thread_info->logger);
			free(message);
			break;

		case F_OPEN:
			file_name = receive_buffer(thread_info->port, thread_info->logger);
			log_info(thread_info->logger, "F_OPEN Kernel con archivo %s", file_name);
			int file_size = open_file(thread_info->logger, file_name);
			package = create_integer_package(F_OPEN, file_size);
			log_info(thread_info->logger, "Se envio el file size %d a Kernel", file_size);
			send_package(package, thread_info->port, thread_info->logger);
			free(file_name);
			break;

		case F_CREATE:
			file_name = receive_buffer(thread_info->port, thread_info->logger);
			log_info(thread_info->logger, "F_CREATE Kernel con archivo %s", file_name);
			int ok = create_file(thread_info->logger, file_name);
			package = create_string_package(F_CREATE, "OK");
			log_info(thread_info->logger, "Se envio el OK a Kernel");
			send_package(package, thread_info->port, thread_info->logger);
			free(file_name);
			break;

		case F_TRUNCATE:
			file_name = "Damian"; // Receive buffer
			file_size = 100;	  // Receive buffer
			log_info(thread_info->logger, "F_TRUNCATE Kernel con archivo %s y tamaño %d", file_name, file_size);
			truncate_file(thread_info->logger, file_name, file_size);
			package = create_string_package(F_TRUNCATE, "OK");
			log_info(thread_info->logger, "Se trunco el archivo y devuelvo OK al Kernel");
			send_package(package, thread_info->port, thread_info->logger);
			// free(file_name);
			break;

		// SWAP
		// Recibo cantidad de bloques (int) - reservo - devuelvo lista?
		case GET_SWAP_BLOCKS:
			int *block_count = (int *)receive_buffer(thread_info->port, thread_info->logger);
			log_debug(thread_info->logger, "GET_SWAP_BLOCKS Memoria necesita %d de bloques SWAP reservados", *block_count);
			t_list *blocks_reserved = reserve_swap_blocks(*block_count);
			// Enviar paquete array
			send_list(GET_SWAP_BLOCKS, blocks_reserved, thread_info->port, thread_info->logger);
			free(block_count);
			break;

		// Recibo bloque a leer (int) - leo data - devuelvo solo el void * con la informacion?
		case GET_FROM_SWAP:
			block = (int *)receive_buffer(thread_info->port, thread_info->logger);
			log_info(thread_info->logger, "Acceso a Bloque SWAP: “Acceso SWAP: %d”", *block);
			void *data = read_from_swap_block(*block);
			// Ver como enviar un void *
			package = create_void_package(GET_FROM_SWAP, fs_config.block_size, data);
			send_package(package, thread_info->port, thread_info->logger);
			free(block);
			break;

		// Recibo t_pag_swap - devuelvo un ok?
		case UPDATE_SWAP:
			page_swap = receive_page_for_swap(thread_info->port, thread_info->logger);
			log_info(thread_info->logger, "Actualizando Bloque SWAP: “Acceso SWAP: %d”", page_swap->swap_block);
			write_to_swap_block(page_swap->swap_block, page_swap->page_content);
			// QUE ENVIAR? YA QUE SOLO ACTUALIZO EN FILESYSTEM
			package = create_integer_package(UPDATE_SWAP, 0);
			send_package(package, thread_info->port, thread_info->logger);
			break;

		// Recibo una lista o que de bloques a liberar - libero - devuelvo ok?
		case FREE_PAGES:
			break;

		default:
			log_error(thread_info->logger, "Unknown OpCode %d - key %s", op_code, thread_info->dict_key);
			dictionary_remove(thread_info->dict, thread_info->dict_key);
			free(thread_info);
			return NULL;
		}
		op_code = receive_op_code(thread_info->port, thread_info->logger);
		if (op_code == -1)
		{
			dictionary_remove(thread_info->dict, thread_info->dict_key);
			free(thread_info);
			return NULL;
		};
	}
	dictionary_remove(thread_info->dict, thread_info->dict_key);
	free(thread_info);
	return NULL;
}

// Return file size si existe
// Return informar que el archivo no existe
int open_file(t_log *logger, char *file_name)
{
	log_info(logger, "Abrir Archivo: %s", file_name);
	t_fcb *file = find_fcb_file(file_name);
	if (file)
	{
		log_info(logger, "Archivo %s abierto", file_name);
		return file->file_size;
	}

	log_info(logger, "Archivo %s no existe", file_name);
	return 0;
}

// Return OK
int create_file(t_log *logger, char *file_name)
{
	log_info(logger, "Crear Archivo: %s", file_name);
	create_fcb_file(file_name);
	return 1;
}

void truncate_file(t_log *logger, char *file_name, int new_size)
{
	log_info(logger, "Truncar Archivo: %s - Tamaño: %d", file_name, new_size);

	t_fcb *fcb = find_fcb_file(file_name);

	// Condicion de incremento, igualdad o decremento del nuevo tamanio

	if (fcb->file_size == new_size)
	{
		log_info(logger, "El nuevo tamaño es el mismo que antes, no se trunco.");
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

		log_debug(logger, "Cantidad de bloques actuales: %d - Cantidad de bloques total nuevos %d", current_size, new_block_count);

		int blocks_to_add = new_block_count - current_size - 1;

		log_debug(logger, "Cantidad de bloques que hay que agregar: %d", blocks_to_add);

		add_blocks(fcb->initial_block, blocks_to_add);
	}
	else
	{
		int current_size = fcb->file_size / sizeof(uint32_t) + 1;
		int blocks_needed = (new_size / sizeof(uint32_t)) + 1;

		log_debug(logger, "Cantidad de bloques actuales: %d - Cantidad de bloques nuevos %d", current_size, blocks_needed);

		free_blocks(fcb->initial_block, blocks_needed);
	}

	fcb->file_size = new_size;

	// Actualizar file FCB
	update_fcb(fcb);
}

// TODO: REVISAR
void wait_in_every_port(int memory, int kernel, t_log *logger)
{
	t_dictionary *dict = dictionary_create();
	for (int i = 0; i < 2; i++)
	{
		pthread_t thread_id;
		t_thread *thread_info = malloc(sizeof(t_thread));
		switch (i)
		{
		case 0:
			thread_info->dict_key = "MEMORY";
			log_trace(logger, "Iniciada thread de Memory");
			thread_info->port = memory;
			break;
		case 1:
			thread_info->dict_key = "KRL";
			log_trace(logger, "Iniciada thread de Kernel");
			thread_info->port = kernel;
			break;
		default:
			break;
		}
		thread_info->logger = logger;
		thread_info->dict = dict;
		dictionary_put(dict, thread_info->dict_key, thread_info->dict_key);
		pthread_create(&thread_id, NULL, (void *)wait_for_commands, thread_info);
		pthread_detach(thread_id);
	}
	while (!dictionary_is_empty(dict))
		;
	dictionary_destroy(dict);
}