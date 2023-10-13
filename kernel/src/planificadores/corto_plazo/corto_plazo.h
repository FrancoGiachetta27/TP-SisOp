#ifndef SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_
#define SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_

#include <initial_configuration/client_start.h>
#include <planificadores/corto_plazo/block/block.h>
#include <planificadores/resources/resources.h>
#include <planificadores/codigos_de_operacion.h>
#include <string.h>

typedef struct {
    t_utils* utils;
    t_conn* conn;
} t_planificador;

typedef struct {
    t_log* logger;
    t_pcb* pcb;
} t_sleep;

pthread_t hilo_planificador_corto_plazo;

void planificador_fifo(t_planificador* info);
void planificador_rr(t_planificador* info);
void planificador_prioridades(t_planificador* info);
void iniciar_planificador_corto_plazo(t_utils* utils, t_conn* conn);

#endif /* SRC_PLANIFICADORES_CORTO_PLAZO_CORTO_PLAZO_H_ */