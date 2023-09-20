#include <command/dispatch.h>

int wait_for_dispatch_command(t_utils* utils, t_conn* ports, int memory_socket, t_reg* registers, int page_size) {
    int op_code = receive_op_code(ports->dispatch_fd, utils->logger);
	if (op_code == -1) return -1;
	while (op_code)
	{
		switch (op_code) {
			case ECHO_CPU_DISPATCH:
				char* message = receive_buffer(ports->dispatch_fd, utils->logger);
				log_info(utils->logger, "OpCode: %d and Message: %s", op_code, message);
				free(message);
                int interrupt_echo_result = wait_for_interrupt_echo(utils, ports);
                if (interrupt_echo_result == -1) return -1;
				log_info(utils->logger, "ECHO to Memory");
				t_package* package = create_string_package(ECHO_MEMORY, "ECHO To Memory from CPU");
				send_package(package, memory_socket, utils->logger);
				break;
			case INSTRUCTION:
				int program_counter = 1;
				int pid = 1;
				char* instruction = fetch(&program_counter, pid, memory_socket, utils->logger);
				t_ins formatted_instruction = decode(instruction, page_size, utils->logger);
				execute(registers, formatted_instruction, utils->logger);
			default:
				log_error(utils->logger, "Unknown OpCode");
				return -1;
		}
		int op_code = receive_op_code(ports->dispatch_fd, utils->logger);
	    if (op_code == -1) return -1;
	}
    return 0;
}