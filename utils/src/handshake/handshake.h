#ifndef SRC_HANDSHAKE_HANDSHAKE_H_
#define SRC_HANDSHAKE_HANDSHAKE_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <commons/log.h>
#include <unistd.h>

enum MODULOS {
	FILESYSTEM = 1,
	CPU = 2,
	KERNEL = 3
};

int receive_handshake(int cliente, t_log* logger);
int send_handshake(int conexion, int handshake, t_log* logger);
int send_handshake_success(int cliente, t_log* logger);
int send_handshake_error(int cliente, t_log* logger);
int check_handshake_result(int servidor, t_log* logger);

#endif /* SRC_HANDSHAKE_HANDSHAKE_H_ */
