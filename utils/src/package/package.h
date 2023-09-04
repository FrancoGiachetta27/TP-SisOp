#ifndef SRC_PACKAGE_PACKAGE_H_
#define SRC_PACKAGE_PACKAGE_H_

#include <stdlib.h>
#include <string.h>
#include <handshake/handshake.h>

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	int op_code;
	t_buffer* buffer;
} t_package;

static void create_buffer(t_package* package);
t_package*  create_package(void);
void send_package(t_package* package, int socket_cliente);
static void* serialize_package(t_package* package, int bytes);
void recieve_package();
void destroy_package(t_package* paquete);

#endif /* SRC_PACKAGE_PACKAGE_H_ */
