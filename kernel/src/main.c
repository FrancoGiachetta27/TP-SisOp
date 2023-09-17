#include <stdlib.h>
#include <stdio.h>
#include <config/config.h>
#include <initial_configuration/client_start.h>
#include <consola/consola.h>
#include <planificadores/planificador.h>

#define LOGS_KERNEL "kernel.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;
	iniciar_planificadores();
	consola_interactiva(); //Mover luego de crear las conexiones


	t_conn* conn = connect_to_modules(utils);
	if (conn == NULL) return EXIT_FAILURE;

	close_conn(conn);
    utils_destroy(utils);
    return 0;
}
