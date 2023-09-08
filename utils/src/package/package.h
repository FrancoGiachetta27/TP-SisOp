#ifndef SRC_PACKAGE_PACKAGE_H_
#define SRC_PACKAGE_PACKAGE_H_

#include <stdlib.h>
#include <string.h>
#include <handshake/handshake.h>

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	int op_code;
	t_buffer* buffer;
} t_package;

void create_buffer(t_package* package);
void* serialize_package(t_package* package, int bytes);
void destroy_package(t_package* paquete);

int receive_op_code(int cliente, t_log* logger);
t_package*  create_package(int op_code);
void add_to_package(t_package* package, void* value, int size);
void send_package(t_package* package, int client_socket, t_log* logger);
void redirect_package(void* info, int receiver_socket, int op_code);
char* receive_package(int client_socket);

#endif /* SRC_PACKAGE_PACKAGE_H_ */
