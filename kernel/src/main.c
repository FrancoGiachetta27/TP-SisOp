//#include <stdlib.h>
//#include <stdio.h>
//#include <config/config.h>
//#include <initial_configuration/client_start.h>
#include <consola/consola.h>
#include <planificadores/largo_plazo/largo_plazo.h>

#define LOGS_KERNEL "kernel.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	iniciar_planificadores(utils);

	t_conn* conn = connect_to_modules(utils);
	iniciar_planificador_largo_plazo();
	consola_interactiva(utils->logger, conn);

	if (conn == NULL) return EXIT_FAILURE;


	log_trace(utils->logger, "Finalizando Kernel");
	close_conn(conn);
    utils_destroy(utils);
    return 0;
}
