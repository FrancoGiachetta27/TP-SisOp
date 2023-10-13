#ifndef SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_
#define SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_

#include <planificadores/planificador.h>

pthread_t largo_plazo_hilo;

void atender_procesos_en_EXIT(t_log* logger);
void planificador_largo_plazo(t_log* logger);
void iniciar_planificador_largo_plazo(t_log* logger);

#endif /* SRC_PLANIFICADORES_LARGO_PLAZO_LARGO_PLAZO_H_ */