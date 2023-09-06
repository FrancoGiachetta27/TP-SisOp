#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <headers/connection.h>
#include <utils/finish_program.h>

#define LOGS_FILE_SYSTEM "filesystem.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (logger == NULL) return EXIT_FAILURE;
	if (argc != NUMERO_DE_ARGUMENTOS_NECESARIOS) {
		log_error(logger, "Cantidad de argumentos invalida.");
	    log_destroy(logger);
	    return EXIT_FAILURE;
	}

	t_config* config = create_config(argv[1], logger);
	if (config == NULL){
		finish_program(logger, config);
		return EXIT_FAILURE;
	}

	if (!check_if_config_value_exists(config, "PUERTO_MEMORIA", logger) || !check_if_config_value_exists(config, "IP_MEMORIA", logger)) {
		finish_program(logger, config);
		return EXIT_FAILURE;
	}
	char* ip = config_get_string_value(config, "IP_MEMORIA");
	char* port = config_get_string_value(config, "PUERTO_MEMORIA");
	int conexion = connect_to_server(ip, port, logger);
	if (conexion == -1) {
		finish_program(logger, config);
		return EXIT_FAILURE;
	}

	int send_handshake_result = send_handshake(conexion, FILESYSTEM, logger);
	if (send_handshake_result == -1) {
		finish_program_with_connection(logger, config, conexion);
		return EXIT_FAILURE;
	}
	int handshake_result = check_handshake_result(conexion, logger);
	if (handshake_result == -1) {
		finish_program_with_connection(logger, config, conexion);
		return EXIT_FAILURE;
	}

	if (!check_if_config_value_exists(config, "PUERTO_ESCUCHA", logger)) {
		finish_program_with_connection(logger, config, conexion);
		return EXIT_FAILURE;
	}
	int server_fd = start_server(config_get_string_value(config, "PUERTO_ESCUCHA"), logger);
	if (server_fd == -1) {
		finish_program_with_connection(logger, config, conexion);
		return EXIT_FAILURE;
	}
	log_trace(logger, "Se inicio correctamente el server");
	int initial_handshake_result = wait_for_initial_handshake(server_fd, logger);
	if (initial_handshake_result == -1) {
		finish_program_with_connection(logger, config, conexion);
		return EXIT_FAILURE;
	}

	finish_program_with_connection(logger, config, conexion);
    return 0;
}
