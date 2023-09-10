#include <echo/echo.h>

void echo(int modulo, t_conn* conn, t_log* logger) {
	t_package* package;
	switch(modulo) {
		case(FS):
			log_info(logger, "ECHO to File System");
			package = create_string_package(ECHO_FILESYSTEM, "ECHO To FileSystem");
			send_package(package, conn->filesystem_socket, logger);
			break;
		case(INTERRUPT):
			log_info(logger, "ECHO to CPU Interrupt");
			package = create_string_package(ECHO_CPU_INTERRUPT, "ECHO To CPU Interrupt");
			send_package(package, conn->cpu_interrupt_socket, logger);
			break;
		case(DISPATCH):
			log_info(logger, "ECHO to CPU Dispatch");
			package = create_string_package(ECHO_CPU_DISPATCH, "ECHO To CPU Dispatch");
			send_package(package, conn->cpu_dispatcher_socket, logger);
			break;
		case(MMY):
			log_info(logger, "ECHO to Memory");
			package = create_string_package(ECHO_MEMORY, "ECHO To Memory");
			send_package(package, conn->memory_socket, logger);
			break;
		default:
			log_error(logger, "Unknown module");
			break;
	}
}


