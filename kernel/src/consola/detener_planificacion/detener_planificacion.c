#include "detener_planificacion.h"

void detener_planificacion(t_log* logger) {
    working = false;
    sem_wait(&planificadores_terminados);
    log_info(logger, "Planificacion Finalizada!");
};