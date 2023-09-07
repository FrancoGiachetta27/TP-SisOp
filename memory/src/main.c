#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>

#define LOGS_MEMORIA "memoria.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define MODULOS_A_CONECTAR 3

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_MEMORIA, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	if (!check_if_config_value_exists(utils, "PUERTO_ESCUCHA")) {
		utils_destroy(utils);
		return EXIT_FAILURE;
	}

	int server_fd = start_server(config_get_string_value(utils->config, "PUERTO_ESCUCHA"), utils->logger);
	if (server_fd == -1) {
		utils_destroy(utils);
		return EXIT_FAILURE;
	}
	log_trace(utils->logger, "Se inicio correctamente");

	int socket_filesystem;
	int socket_cpu;
	int socket_kernel;
	int connected_modules;
	for (connected_modules = 0; connected_modules < MODULOS_A_CONECTAR; connected_modules++) {
		int socket_cliente = wait_for_client(server_fd, utils->logger);
		if (socket_cliente == -1) break;
		int handshake = receive_handshake(socket_cliente, utils->logger);
		if (handshake == -1) break;
		switch(handshake) {
			case FILESYSTEM:
				socket_filesystem = socket_cliente;
				send_handshake_success(socket_filesystem, utils->logger);
				log_trace(utils->logger, "Se conecto el FileSystem");
				break;
			case CPU:
				socket_cpu = socket_cliente;
				send_handshake_success(socket_cpu, utils->logger);
				log_trace(utils->logger, "Se conecto la CPU");
				break;
			case KERNEL:
				socket_kernel = socket_cliente;
				send_handshake_success(socket_kernel, utils->logger);
				log_trace(utils->logger, "Se conecto el Kernel");
				break;
			default:
				send_handshake_error(socket_cliente, utils->logger);
				close(socket_cliente);
				break;
		}
	}
	if (connected_modules < MODULOS_A_CONECTAR) {
		utils_destroy(utils);
		return EXIT_FAILURE;
	}
	log_trace(utils->logger, "Se termina el programa");
	utils_destroy(utils);
    return 0;
}
