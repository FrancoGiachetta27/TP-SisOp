#include "iniciar_planificacion.h"

void iniciar_planificacion(t_utils* utils, t_conn* conn) {
    sem_destroy(&planificadores_terminados);
    sem_init(&planificadores_terminados,0,PLANNING_THREADS_INITIAL_VALUE);
    working = true;
    iniciar_planificador_largo_plazo(utils->logger);
	iniciar_planificador_corto_plazo(utils, conn);
    log_info(utils->logger, "Planificacion Iniciada!");
}