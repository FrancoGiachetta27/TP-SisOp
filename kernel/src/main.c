#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <logger/logger.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <utils/finish_program.h>
#include <unistd.h>
#include "consola/consola.h"

#define LOGS_KERNEL "kernel.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define NUMERO_DE_CONNECTION_KEYS 8

int main(int argc, char* argv[]) {
	t_log* logger = create_logger(LOGS_KERNEL, true, LOG_LEVEL_TRACE);
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
	int connected_modules;
	consola_interactiva();
	for (connected_modules = 0; connected_modules < NUMERO_DE_CONNECTION_KEYS; connected_modules = connected_modules + 2) {
		char* port_key = connection_config[connected_modules];
		char* ip_key = connection_config[connected_modules + 1];
		if (!check_if_config_value_exists(config, port_key, logger) || !check_if_config_value_exists(config, ip_key, logger)) break;
		char* port = config_get_string_value(config, port_key);
		char* ip = config_get_string_value(config, ip_key);
		int conexion = connect_to_server(ip, port, logger);
		if (conexion == -1) break;
		int handshake_send_result = send_handshake(conexion, KERNEL, logger);
		if (handshake_send_result == -1) break;
		int handshake_result = check_handshake_result(conexion, logger);
		if (handshake_result == -1) break;
		switch (connected_modules) {
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
	if (connected_modules != NUMERO_DE_CONNECTION_KEYS) {
	    finish_program(logger, config);
		if (connected_modules >= 2) close(memory_socket);
		if (connected_modules >= 4) close(filesystem_socket);
		if (connected_modules >= 6) close(cpu_dispatcher_socket);
	    return EXIT_FAILURE;
	}
	close(memory_socket);
	close(filesystem_socket);
	close(cpu_dispatcher_socket);
	close(cpu_interrupt_socket);
    finish_program(logger, config);
    return 0;
}
