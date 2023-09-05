#include "package.h"

void create_buffer(t_package* package)
{
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->size = 0;
	package->buffer->stream = NULL;
}

t_package* create_package(int op_code)
{
	t_package* package = malloc(sizeof(t_package));
	package->op_code = op_code;
	create_buffer(package);
	return package;
}


void add_to_package(t_package* package, void* value, int size)
{
	package->buffer->stream = realloc(package->buffer->stream, package->buffer->size + size + sizeof(int));

	memcpy(package->buffer->stream + package->buffer->size, &size, sizeof(int));
	memcpy(package->buffer->stream + package->buffer->size + sizeof(int), value, size);

	package->buffer->size += size + sizeof(int);
}

void send_package(t_package* package, int client_socket, t_log* logger)
{
	int bytes = package->buffer->size + 2 * sizeof(int);
	void* a_enviar = serialize_package(package, bytes);

	int send_result = send(client_socket, a_enviar, bytes, 0);

	if (send_result == -1) {
		log_error(logger, "Error al enviar el paquete");
		abort();
	}

	free(a_enviar);
	destroy_package(package);
}

char* receive_package(int socket_cliente)
{
	int size;
	int offset = 0;
	void * buffer;
	int buffer_size;

	buffer = receive_buffer(&size, socket_cliente);

	memcpy(&buffer_size, buffer + offset, sizeof(int));
	offset+=sizeof(int);
	char* value = malloc(buffer_size);
	memcpy(value, buffer+offset, buffer_size);
	offset+=buffer_size;

	free(buffer);
	return value;
}

void* serialize_package(t_package* package)
{
	int bytes = package->buffer->size + 2 * sizeof(int);

	void * serde_pkg = malloc(bytes);
	int desplazamiento = 0;

	memcpy(serde_pkg + desplazamiento, &(package->op_code), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_pkg + desplazamiento, &(package->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_pkg + desplazamiento, package->buffer->stream, package->buffer->size);
	desplazamiento+= package->buffer->size;

	return serde_pkg;
}

void* receive_buffer(int* size, int client_socket)
{
	void *buffer;

	recv(client_socket, size, sizeof(int), MSG_WAITALL);

	buffer = malloc(*size);
	recv(client_socket, buffer, *size, MSG_WAITALL);

	return buffer;
}

void destroy_package(t_package* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
