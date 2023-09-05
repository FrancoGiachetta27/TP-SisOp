#include <headers/connection.h>

int wait_for_initial_handshake(int server_fd, t_log* logger) {
	int socket_kernel = wait_for_client(server_fd, logger);
	int handshake = receive_handshake(socket_kernel, logger);
	if (handshake == KERNEL) {
		send_handshake_success(socket_kernel, logger);
		log_trace(logger, "Se conecto al Kernel");
	} else {
		send_handshake_error(socket_kernel, logger);
		close(socket_kernel);
	}
	return socket_kernel;
}