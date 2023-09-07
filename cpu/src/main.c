#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <headers/connection.h>

#define LOGS_CPU "cpu.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define NUMERO_DE_PORTS 2

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_CPU, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	if (!check_if_config_value_exists(utils, "IP_MEMORIA") || !check_if_config_value_exists(utils, "PUERTO_MEMORIA")) {
	    utils_destroy(utils);
	    return EXIT_FAILURE;
	}

	char* ip = config_get_string_value(utils->config, "IP_MEMORIA");
	char* port = config_get_string_value(utils->config, "PUERTO_MEMORIA");
	int conexion = connect_to_server(ip, port, utils->logger);
	if (conexion == -1) {
		utils_destroy_with_connection(utils, conexion);
	    return EXIT_FAILURE;
	}

	int handshake_send_result = send_handshake(conexion, CPU, utils->logger);
	if (handshake_send_result == -1) {
		utils_destroy_with_connection(utils, conexion);
	    return EXIT_FAILURE;
	}
	int handshake_result = check_handshake_result(conexion, utils->logger);
	if (handshake_result == -1) {
		utils_destroy_with_connection(utils, conexion);
	    return EXIT_FAILURE;
	}

	int server_dispatch_fd;
	int server_interrupt_fd;
	char* port_keys[] = { "PUERTO_ESCUCHA_DISPATCH", "PUERTO_ESCUCHA_INTERRUPT" };
	for (int i = 0; i < NUMERO_DE_PORTS; i++) {
		char* port_string = port_keys[i];
		if (!check_if_config_value_exists(utils, port_string)) {
			utils_destroy(utils);
			abort();
		}
		int fd = start_server(config_get_string_value(utils->config, port_string), utils->logger);
		switch (i) {
			case 0:
				server_dispatch_fd = fd;
				log_trace(utils->logger, "Se inicio correctamente el server de dispatch");
			break;
			case 1:
				server_interrupt_fd = fd;
				log_trace(utils->logger, "Se inicio correctamente el server de interrupt");
			break;
		}
	}

	int result_dispatch_handshake = wait_for_initial_handshake(server_dispatch_fd, utils->logger);
	if (result_dispatch_handshake == -1) {
		utils_destroy_with_connection(utils, conexion);
	    return EXIT_FAILURE;
	}
	int result_interrupt_handshake = wait_for_initial_handshake(server_interrupt_fd, utils->logger);
	if (result_interrupt_handshake == -1) {
		utils_destroy_with_connection(utils, conexion);
	    return EXIT_FAILURE;
	}

    return 0;
}
