#ifndef SRC_HEADERS_CONNECTION_H_
#define SRC_HEADERS_CONNECTION_H_

#include <handshake/handshake.h>
#include <connection/connection.h>
#include <commons/log.h>
#include <unistd.h>

enum OP_CODE_FS {
	// CPU_MEMORY = ...
	KERNEL_FILESYSTEM = 3,
	FILESYSTEM_MEMORY = 4,
	MEMORY_FILESYSTEM = 5
};

int wait_for_initial_handshake(int server_fd, t_log* logger);

#endif /* SRC_HEADERS_CONNECTION_H_ */
