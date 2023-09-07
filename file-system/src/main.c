#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <headers/connection.h>

#define LOGS_FILE_SYSTEM "filesystem.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	if (!check_if_config_value_exists(utils, "PUERTO_MEMORIA") || !check_if_config_value_exists(utils, "IP_MEMORIA")) {
		utils_destroy(utils);
		return EXIT_FAILURE;
	}
	char* ip = config_get_string_value(utils->config, "IP_MEMORIA");
	char* port = config_get_string_value(utils->config, "PUERTO_MEMORIA");
	int conexion = connect_to_server(ip, port, utils->logger);
	if (conexion == -1) {
		utils_destroy(utils);
		return EXIT_FAILURE;
	}

	int send_handshake_result = send_handshake(conexion, FILESYSTEM, utils->logger);
	if (send_handshake_result == -1) {
		utils_destroy_with_connection(utils, conexion);
		return EXIT_FAILURE;
	}
	int handshake_result = check_handshake_result(conexion, utils->logger);
	if (handshake_result == -1) {
		utils_destroy_with_connection(utils, conexion);
		return EXIT_FAILURE;
	}

	if (!check_if_config_value_exists(utils, "PUERTO_ESCUCHA")) {
		utils_destroy_with_connection(utils, conexion);
		return EXIT_FAILURE;
	}
	int server_fd = start_server(config_get_string_value(utils->config, "PUERTO_ESCUCHA"), utils->logger);
	if (server_fd == -1) {
		utils_destroy_with_connection(utils, conexion);
		return EXIT_FAILURE;
	}
	log_trace(utils->logger, "Se inicio correctamente el server");
	int initial_handshake_result = wait_for_initial_handshake(server_fd, utils->logger);
	if (initial_handshake_result == -1) {
		utils_destroy_with_connection(utils, conexion);
		return EXIT_FAILURE;
	}

	utils_destroy_with_connection(utils, conexion);
    return 0;
}
