#ifndef SRC_CONSOLA_FINALIZAR_PROCESO_FINALIZAR_PROCESO_H_
#define SRC_CONSOLA_FINALIZAR_PROCESO_FINALIZAR_PROCESO_H_

#include "planificadores/planificador.h"

void finalizar_proceso(uint32_t pid);
void eliminar_proceso(t_pcb* pcb);
t_pcb* encontrar_proceso_por_PID(uint32_t pid);

#endif /* SRC_CONSOLA_FINALIZAR_PROCESO_FINALIZAR_PROCESO_H_ */
