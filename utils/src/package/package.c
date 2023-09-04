#include "package.h"

static void create_buffer(t_package* package)
{
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->size = 0;
	package->buffer->stream = NULL;
}

t_package* create_package(void)
{
	t_package* package = malloc(sizeof(t_package));
	package->op_code = PACKAGE;
	create_buffer(package);
	return package;
}

void send_package(t_package* package, int socket_cliente)
{
	int bytes = package->buffer->size + 2 * sizeof(int);
	void* a_enviar = serialize_package(package, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

static void* serialize_package(t_package* package, int bytes)
{
	void * serde_package = malloc(bytes);
	int desplazamiento = 0;

	memcpy(serde_package + desplazamiento, &(package->op_code), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_package + desplazamiento, &(package->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(serde_package + desplazamiento, package->buffer->stream, package->buffer->size);
	desplazamiento+= package->buffer->size;

	return serde_package;
}

void recieve_package()
{
}

void destroy_package(t_package* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
