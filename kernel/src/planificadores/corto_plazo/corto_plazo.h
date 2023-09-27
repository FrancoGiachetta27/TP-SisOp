#ifndef SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_
#define SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_

#include <initial_configuration/client_start.h>
#include <planificadores/planificador.h>
#include <planificadores/codigos_de_operacion.h>
#include <string.h>

typedef struct {
    t_utils* utils;
    t_conn* conn;
} t_planificador;

void planificador_fifo(t_planificador* info);
void planificador_rr(t_planificador* info);
void planificador_prioridades(t_planificador* info);
void iniciar_planificador_corto_plazo(t_utils* utils, t_conn* conn);

#endif /* SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_ */