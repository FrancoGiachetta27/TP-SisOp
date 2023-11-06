#include <command/command.h>
#include <fcntl.h>
#include <fcb/fcb.h>
#include "initial_configuration/fs_config.h"

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
		switch (op_code)
		{
		case ECHO_FILESYSTEM:
			char *message = receive_buffer(socket_kernel, utils->logger);
			log_info(utils->logger, "OpCode: %d and Message: %s", op_code, message);
			free(message);
			t_package *package = create_string_package(ECHO_MEMORY, "ECHO To Memory From FS");
			send_package(package, memory_socket, utils->logger);
			break;
		case F_OPEN:
			char *file_name = receive_buffer(socket_kernel, utils->logger);
			log_info(utils->logger, "F_OPEN Kernel con archivo %s", file_name);
			int file_size = open_file(utils, file_name);
			package = create_integer_package(F_OPEN, file_size);
			send_package(package, socket_kernel, utils->logger);
			free(file_name);
			break;
		case F_CREATE:
			file_name = receive_buffer(socket_kernel, utils->logger);
			log_debug(utils->logger, "F_CREATE Kernel con archivo %s", file_name);
			int ok = create_file(utils, file_name);
			package = create_string_package(F_CREATE, "OK");
			free(file_name);
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

// MODIFICAR LOS RETURNS

int open_file(t_utils *utils, char *file_name)
{
	log_info(utils->logger, "Abrir Archivo: %s", file_name);
	t_fcb *file = find_fcb_file(file_name);
	if (file)
	{
		log_info(utils->logger, "Si existe el archivo: %s", file_name);
		printf("%d\n", file->file_size);
		return file->file_size;
	}

	log_info(utils->logger, "Archivo %s no existe", file_name);
	return 0;
}

int create_file(t_utils *utils, char *file_name) {
	log_info(utils->logger, "Crear Archivo: %s", file_name);
	create_fcb_file(file_name);
	return 1;
}

// VER CON EZE
// el definir que devuelvan el tamaño es para que lo carguen en la tabla de archivos abiertos, después queda en uds si 
// lo guardan o no y si le indican al FS si ante un truncate el FS tiene que ampliar o reducir el archivo desde el Kernel o si delegan todo eso en el FS.
// Lo realmente importante del Abrir archivo en el FS es checkear si existe o no.
void truncate_file(t_utils *utils, char *file_name, int new_size) {
	log_info(utils->logger, "Truncar Archivo: %s - Tamaño: %d", file_name, new_size);

	t_fcb *fcb = find_fcb_file(file_name);

	// Condicion de incremento, igualdad o decremento del nuevo tamanio

	if (fcb->file_size == new_size) {
		log_info(utils->logger, "El nuevo tamaño es el mismo que antes, no se trunco.");
	}

	// Ver logica de asignar bloques
	if (fcb->file_size > new_size) {

	} else {

	}

	fcb->file_size = new_size;
	fcb->initial_block = 3;

	// Actualizar FCB
	update_fcb(fcb);

}