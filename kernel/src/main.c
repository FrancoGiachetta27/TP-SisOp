#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <config/config.h>
#include <connection/connection.h>
#include <handshake/handshake.h>
#include <unistd.h>
#include "consola/consola.h"

#define LOGS_KERNEL "kernel.log"
#define NUMERO_DE_ARGUMENTOS_NECESARIOS 2
#define NUMERO_DE_CONNECTION_KEYS 8

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;
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
		if (!check_if_config_value_exists(utils, port_key) || !check_if_config_value_exists(utils, ip_key)) break;
		char* port = config_get_string_value(utils->config, port_key);
		char* ip = config_get_string_value(utils->config, ip_key);
		int conexion = connect_to_server(ip, port, utils->logger);
		if (conexion == -1) break;
		int handshake_send_result = send_handshake(conexion, KERNEL, utils->logger);
		if (handshake_send_result == -1) break;
		int handshake_result = check_handshake_result(conexion, utils->logger);
		if (handshake_result == -1) break;
		switch (connected_modules) {
			case 0:
				memory_socket = conexion;
				log_trace(utils->logger, "Conectado a Memory");
			break;
			case 2:
				filesystem_socket = conexion;
				log_trace(utils->logger, "Conectado al FileSystem");
			break;
			case 4:
				cpu_dispatcher_socket = conexion;
				log_trace(utils->logger, "Conectado al Dispatcher de la CPU");
			break;
			case 6:
				cpu_interrupt_socket = conexion;
				log_trace(utils->logger, "Conectado al Interrupt de la CPU");
			break;
		}
	}
	if (connected_modules != NUMERO_DE_CONNECTION_KEYS) {
		utils_destroy(utils);
		if (connected_modules >= 2) close(memory_socket);
		if (connected_modules >= 4) close(filesystem_socket);
		if (connected_modules >= 6) close(cpu_dispatcher_socket);
	    return EXIT_FAILURE;
	}
	close(memory_socket);
	close(filesystem_socket);
	close(cpu_dispatcher_socket);
	close(cpu_interrupt_socket);
    utils_destroy(utils);
    return 0;
}
