#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <utils/finish_program.h>

#define LOGS_MEMORIA "memoria.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define MODULOS_A_CONECTAR 3

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (logger == NULL) return EXIT_FAILURE;
	if (argc != NUMERO_DE_ARGUMENTOS_NECESARIOS) {
		log_error(logger, "Cantidad de argumentos invalida.");
	    log_destroy(logger);
	    return EXIT_FAILURE;
	}

	t_config* config = create_config(argv[1], logger);
	if (config == NULL) {
		log_destroy(logger);
		return EXIT_FAILURE;
	}
	if (!check_if_config_value_exists(config, "PUERTO_ESCUCHA", logger)) {
		finish_program(logger, config);
		return EXIT_FAILURE;
	}
	int server_fd = start_server(config_get_string_value(config, "PUERTO_ESCUCHA"), logger);
	if (server_fd == -1) {
		finish_program(logger, config);
		return EXIT_FAILURE;
	}
	log_trace(logger, "Se inicio correctamente");

	int socket_filesystem;
	int socket_cpu;
	int socket_kernel;
	int connected_modules;
	for (connected_modules = 0; connected_modules < MODULOS_A_CONECTAR; connected_modules++) {
		int socket_cliente = wait_for_client(server_fd, logger);
		if (socket_cliente == -1) break;
		int handshake = receive_handshake(socket_cliente, logger);
		if (handshake == -1) break;
		switch(handshake) {
			case FILESYSTEM:
				socket_filesystem = socket_cliente;
				send_handshake_success(socket_filesystem, logger);
				log_trace(logger, "Se conecto el FileSystem");
				break;
			case CPU:
				socket_cpu = socket_cliente;
				send_handshake_success(socket_cpu, logger);
				log_trace(logger, "Se conecto la CPU");
				break;
			case KERNEL:
				socket_kernel = socket_cliente;
				send_handshake_success(socket_kernel, logger);
				log_trace(logger, "Se conecto el Kernel");
				break;
			default:
				send_handshake_error(socket_cliente, logger);
				close(socket_cliente);
				break;
		}
	}
	if (connected_modules < MODULOS_A_CONECTAR) {
		finish_program(logger, config);
		return EXIT_FAILURE;
	}
	log_trace(logger, "Se termina el programa");
	finish_program(logger, config);
    return 0;
}
