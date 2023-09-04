#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <unistd.h>

#define LOGS_MEMORIA "memoria.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define MODULOS_A_CONECTAR 3

void finish_program(t_log* logger, t_config* config);

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (argc != NUMERO_DE_ARGUMENTOS_NECESARIOS) {
		log_error(logger, "Cantidad de argumentos invalida.");
	    log_destroy(logger);
	    return EXIT_FAILURE;
	}

	t_config* config = create_config(argv[1], logger);
	check_if_config_value_exists(config, "PUERTO_ESCUCHA", logger);
	int server_fd = start_server(config_get_string_value(config, "PUERTO_ESCUCHA"), logger);
	log_trace(logger, "Se inicio correctamente");

	int socket_filesystem;
	int socket_cpu;
	int socket_kernel;
	for (int i = 0; i < MODULOS_A_CONECTAR; i++) {
		int socket_cliente = wait_for_client(server_fd, logger);
		int handshake = receive_handshake(socket_cliente, logger);
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
	log_trace(logger, "Se termina el programa");
	finish_program(logger, config);
    return 0;
}

void finish_program(t_log* logger, t_config* config) {
	log_destroy(logger);
	config_destroy(config);
}
