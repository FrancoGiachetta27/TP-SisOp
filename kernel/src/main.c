#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <package/package.h>
#include <headers/connection.h>
#include <unistd.h>

#define LOGS_KERNEL "kernel.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define NUMERO_DE_CONNECTION_KEYS 8

void finish_program(t_log* logger, t_config* config);

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (argc != NUMERO_DE_ARGUMENTOS_NECESARIOS) {
		log_error(logger, "Cantidad de argumentos invalida.");
	    log_destroy(logger);
	    return EXIT_FAILURE;
	}

	t_config* config = create_config(argv[1], logger);

	char* connection_config[] = {
			"PUERTO_MEMORIA", "IP_MEMORIA",
			"PUERTO_FILESYSTEM", "IP_FILESYSTEM",
			"PUERTO_CPU_DISPATCH", "IP_CPU",
			"PUERTO_CPU_INTERRUPT", "IP_CPU"
	};

	int memory_socket;
	int filesystem_socket;
	int cpu_dispatcher_socket;
	int cpu_interrupt_socket;
	for (int i = 0; i < NUMERO_DE_CONNECTION_KEYS; i = i + 2) {
		char* port = config_get_string_value(config, connection_config[i]);
		char* ip = config_get_string_value(config, connection_config[i + 1]);
		int conexion = connect_to_server(ip, port, logger);
		send_handshake(conexion, KERNEL, logger);
		check_handshake_result(conexion, logger);
		switch (i) {
			case 0:
				memory_socket = conexion;
				log_trace(logger, "Conectado a Memory");
			break;
			case 2:
				filesystem_socket = conexion;
				log_trace(logger, "Conectado al FileSystem");
			break;
			case 4:
				cpu_dispatcher_socket = conexion;
				log_trace(logger, "Conectado al Dispatcher de la CPU");
			break;
			case 6:
				cpu_interrupt_socket = conexion;
				log_trace(logger, "Conectado al Interrupt de la CPU");
			break;
		}
	}

		// Kernel - Memoria
//		t_package* package = create_package(KERNEL_MEMORY);
//
//		char* message_kernel_memory = "hola memoria, soy kernel";
//
//		add_to_package(package, message_kernel_memory , strlen(message_kernel_memory )+1);
//
//		send_package(package, memory_socket, logger);

		// Kernel - CPU - Memoria
//		t_package* package = create_package(KERNEL_MEMORY);
//
//		char* message_kernel_cpu_memory = "hola memoria, soy CPU, me mando kernel";
//
//		add_to_package(package, message_kernel_cpu_memory, strlen(message_kernel_cpu_memory)+1);
//
//		send_package(package, cpu_socket, logger);

		// Kernel - FS - Memoria

		t_package* package = create_package(FILESYSTEM_MEMORY);

		char* message_kernel_fs_memory = "hola memoria, soy file system, me mando kernel";

		add_to_package(package, message_kernel_fs_memory, strlen(message_kernel_fs_memory)+1);

		send_package(package, filesystem_socket, logger);

	close(memory_socket);
	close(filesystem_socket);
	close(cpu_dispatcher_socket);
	close(cpu_interrupt_socket);
    finish_program(logger, config);
    return 0;
}

void finish_program(t_log* logger, t_config* config) {
	log_destroy(logger);
	config_destroy(config);
}
