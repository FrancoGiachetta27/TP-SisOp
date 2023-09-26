#ifndef SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_
#define SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_

#include <planificadores/planificador.h>

void atender_procesos_en_EXIT();
void planificador_largo_plazo();
void iniciar_planificador_largo_plazo();
void eliminar_proceso(t_pcb* pcb);

#endif /* SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_ */