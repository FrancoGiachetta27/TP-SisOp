#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <headers/connection.h>
#include <unistd.h>

#define LOGS_FILE_SYSTEM "filesystem.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2

void finish_program(t_log* logger, t_config* config, int conexion);

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_FILE_SYSTEM, true, LOG_LEVEL_TRACE);
	if (argc != NUMERO_DE_ARGUMENTOS_NECESARIOS) {
		log_error(logger, "Cantidad de argumentos invalida.");
	    log_destroy(logger);
	    return EXIT_FAILURE;
	}

	t_config* config = create_config(argv[1], logger);
	check_if_config_value_exists(config, "PUERTO_MEMORIA", logger);
	check_if_config_value_exists(config, "IP_MEMORIA", logger);
	char* ip = config_get_string_value(config, "IP_MEMORIA");
	char* port = config_get_string_value(config, "PUERTO_MEMORIA");
	int conexion = connect_to_server(ip, port, logger);

	send_handshake(conexion, FILESYSTEM, logger);
	check_handshake_result(conexion, logger);

	check_if_config_value_exists(config, "PUERTO_ESCUCHA", logger);
	int server_fd = start_server(config_get_string_value(config, "PUERTO_ESCUCHA"), logger);
	log_trace(logger, "Se inicio correctamente el server");
	int socket_kernel = wait_for_initial_handshake(server_fd, logger);

	int handshake = receive_handshake(socket_kernel,logger);
	switch(handshake)  {
		case KERNEL_FILESYSTEM:
			char* message = receive_package(socket_kernel);

				break;
			default:
				log_error(logger, "Error al interseptar el paquete");
			}

	finish_program(logger, config, conexion);
    return 0;
}

void finish_program(t_log* logger, t_config* config, int conexion) {
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
