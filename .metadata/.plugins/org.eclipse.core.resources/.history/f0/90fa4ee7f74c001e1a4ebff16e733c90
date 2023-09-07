#ifndef SRC_HANDSHAKE_HANDSHAKE_H_
#define SRC_HANDSHAKE_HANDSHAKE_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <commons/log.h>
#include <unistd.h>

enum MODULOS {
	FILESYSTEM = 1,
	CPU = 2,
	KERNEL = 3,
	KERNEL_CPU = 4,
	KERNEL_FILESYSTEM = 5,
	KERNEL_MEMORY = 6,
	CPU_MEMORY = 7,
	FILESYSTEM_MEMORY = 8,
	MEMORY_FILESYSTEM = 9
};

int receive_handshake(int cliente, t_log* logger);
void send_handshake(int conexion, int handshake, t_log* logger);
void send_handshake_success(int cliente, t_log* logger);
void send_handshake_error(int cliente, t_log* logger);
void check_handshake_result(int servidor, t_log* logger);

#endif /* SRC_HANDSHAKE_HANDSHAKE_H_ */
