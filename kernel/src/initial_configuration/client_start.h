#ifndef SRC_INITIAL_CONFIGURATION_CLIENT_START_H_
#define SRC_INITIAL_CONFIGURATION_CLIENT_START_H_

#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>

#define NUMERO_DE_CONNECTION_KEYS 8

typedef enum {
	KERNEL_MEMORY = 0,
	KERNEL_CPU,
	KERNEL_FILESYSTEM = 2,
	FILESYSTEM_MEMORY = 3,
	CPU_MEMORY,
} op_code_kernel;

typedef struct {
	int memory_socket;
	int filesystem_socket;
	int cpu_dispatcher_socket;
	int cpu_interrupt_socket;
} t_conn;

t_conn* connect_to_modules(t_utils* utils);
void close_conn(t_conn* conn);

#endif /* SRC_INITIAL_CONFIGURATION_CLIENT_START_H_ */
