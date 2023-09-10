#include <package/package.h>

t_package* create_empty_package(int op_code)
{
	t_package* package = malloc(sizeof(t_package));
	package->op_code = op_code;
    package->size = 0;
	package->buffer = NULL;
	return package;
}

t_package* create_string_package(int op_code, char* message)
{
	t_package* package = create_empty_package(op_code);
    package->size = strlen(message) + 1;
	package->buffer = message;
	return package;
}

void* serialize_package(t_package* package,int bytes)
{
	void * serde_pkg = malloc(bytes);
	int desplazamiento = 0;

	memcpy(serde_pkg + desplazamiento, &(package->op_code), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_pkg + desplazamiento, &(package->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_pkg + desplazamiento, package->buffer, package->size);
	desplazamiento+= package->size;

	return serde_pkg;
}

void send_package(t_package* package, int client_socket, t_log* logger)
{
	int bytes = package->size + 2 * sizeof(int);
	void* a_enviar = serialize_package(package, bytes);
	if (send(client_socket, a_enviar, bytes, 0) == -1) {
		log_error(logger, "Error al enviar el paquete");
		abort();
	}
	log_trace(logger, "Se ha enviado el paquete con opCode %d", package->op_code);
	free(a_enviar);
	destroy_package(package);
}

void destroy_package(t_package* paquete)
{
	free(paquete);
}

int receive_op_code(int cliente, t_log* logger) {
	int op_code;
	if (check_recv(recv(cliente, &op_code, sizeof(int), MSG_WAITALL), logger) != 0) return -1;
	return op_code;
}

void* receive_buffer(int client_socket, t_log* logger)
{
	int size;
	void* buffer;
	if (check_recv(recv(client_socket, &size, sizeof(int), MSG_WAITALL), logger) != 0) return NULL;
	buffer = malloc(size);
	if (check_recv(recv(client_socket, buffer, size, MSG_WAITALL), logger) != 0) return NULL;
	return buffer;
}