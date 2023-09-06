#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <headers/connection.h>
#include <utils/finish_program.h>

#define LOGS_CPU "cpu.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define NUMERO_DE_PORTS 2

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_CPU, true, LOG_LEVEL_TRACE);
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
	if (!check_if_config_value_exists(config, "IP_MEMORIA", logger) || !check_if_config_value_exists(config, "PUERTO_MEMORIA", logger)) {
	    finish_program(logger, config);
	    return EXIT_FAILURE;
	}
	char* ip = config_get_string_value(config, "IP_MEMORIA");
	char* port = config_get_string_value(config, "PUERTO_MEMORIA");
	int conexion = connect_to_server(ip, port, logger);
	if (conexion == -1) {
		finish_program_with_connection(logger, config, conexion);
	    return EXIT_FAILURE;
	}

	int handshake_send_result = send_handshake(conexion, CPU, logger);
	if (handshake_send_result == -1) {
		finish_program_with_connection(logger, config, conexion);
	    return EXIT_FAILURE;
	}
	int handshake_result = check_handshake_result(conexion, logger);
	if (handshake_result == -1) {
		finish_program_with_connection(logger, config, conexion);
	    return EXIT_FAILURE;
	}

	int server_dispatch_fd;
	int server_interrupt_fd;
	char* port_keys[] = { "PUERTO_ESCUCHA_DISPATCH", "PUERTO_ESCUCHA_INTERRUPT" };
	for (int i = 0; i < NUMERO_DE_PORTS; i++) {
		char* port_string = port_keys[i];
		if (!check_if_config_value_exists(config, port_string, logger)) {
			finish_program(logger, config);
			abort();
		}
		int fd = start_server(config_get_string_value(config, port_string), logger);
		switch (i) {
			case 0:
				server_dispatch_fd = fd;
				log_trace(logger, "Se inicio correctamente el server de dispatch");
			break;
			case 1:
				server_interrupt_fd = fd;
				log_trace(logger, "Se inicio correctamente el server de interrupt");
			break;
		}
	}

	int result_dispatch_handshake = wait_for_initial_handshake(server_dispatch_fd, logger);
	if (result_dispatch_handshake == -1) {
		finish_program_with_connection(logger, config, conexion);
	    return EXIT_FAILURE;
	}
	int result_interrupt_handshake = wait_for_initial_handshake(server_interrupt_fd, logger);
	if (result_interrupt_handshake == -1) {
		finish_program_with_connection(logger, config, conexion);
	    return EXIT_FAILURE;
	}

    return 0;
}
