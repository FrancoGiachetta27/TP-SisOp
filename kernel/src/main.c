#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <initial_configuration/client_start.h>
#include <consola/consola.h>
#include <package/package.h>

#define LOGS_KERNEL "kernel.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;
	//consola_interactiva();

	t_conn* conn = connect_to_modules(utils);
	if (conn == NULL) return EXIT_FAILURE;

	t_package* package = create_package(FILESYSTEM_MEMORY);

//	char* message = "hola memoria";
//
//	add_to_package(package, message, strlen(message) + 1);
//
//	send_package(package, conn->memory_socket, utils->logger);

	char* message = "hola memoria, soy filesystem";

	add_to_package(package, message, strlen(message) + 1);

	send_package(package, conn->filesystem_socket, utils->logger);

	log_info(utils->logger, "%d", conn->filesystem_socket);

	close_conn(conn);
    utils_destroy(utils);
    return 0;
}
