//#include <stdlib.h>
//#include <stdio.h>
//#include <config/config.h>
//#include <initial_configuration/client_start.h>
#include <consola/consola.h>
#include <planificadores/largo_plazo/largo_plazo.h>
#include <planificadores/corto_plazo/corto_plazo.h>

#define LOGS_KERNEL "kernel.log"

int main(int argc, char* argv[]) {
	t_utils* utils = create_initial_config(argc, argv, LOGS_KERNEL, true, LOG_LEVEL_TRACE);
	if (utils == NULL) return EXIT_FAILURE;

	iniciar_estructuras_planificadores(utils);

	t_conn* conn = connect_to_modules(utils);
	if (conn == NULL) return EXIT_FAILURE;

	iniciar_planificador_largo_plazo(utils->logger);
	iniciar_planificador_corto_plazo(utils, conn);
	consola_interactiva(utils->logger, conn);

	log_trace(utils->logger, "Finalizando Kernel");
	terminar_estructuras_planificadores();
	close_conn(conn);
    utils_destroy(utils);
    return 0;
}
