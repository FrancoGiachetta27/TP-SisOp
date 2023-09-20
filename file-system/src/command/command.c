#include <command/command.h>

int wait_for_commands(int socket_kernel, int memory_socket, t_utils* utils) {
    int op_code = receive_op_code(socket_kernel, utils->logger);
	if (op_code == -1) {
		 return -1;
	}
	while (op_code)
	{
		switch (op_code) {
			case ECHO_FILESYSTEM:
				char* message = receive_buffer(socket_kernel, utils->logger);
				log_info(utils->logger, "OpCode: %d and Message: %s", op_code, message);
				free(message);
				t_package* package = create_string_package(ECHO_MEMORY, "ECHO To Memory From FS");
				send_package(package, memory_socket, utils->logger);
				break;
			default:
				log_error(utils->logger, "Unknown OpCode");
				utils_destroy_with_connection(utils, memory_socket);
				return -1;
		}
		int op_code = receive_op_code(socket_kernel, utils->logger);
		if (op_code == -1) return -1;
	}
    return 0;
}