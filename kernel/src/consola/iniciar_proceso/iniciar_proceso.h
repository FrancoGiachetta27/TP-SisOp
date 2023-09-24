#ifndef SRC_CONSOLA_INICIAR_PROCESO_INICIAR_PROCESO_H_
#define SRC_CONSOLA_INICIAR_PROCESO_INICIAR_PROCESO_H_

#include <stdint.h>
#include <pcb/pcb.h>
#include "initial_configuration/client_start.h"
#include "planificadores/planificador.h"

void iniciar_proceso(char* source, char* size, char* priority, t_log* logger, t_conn* conn);
void avisar_a_memoria_nuevo_proceso(uint32_t id, uint32_t tamanio, char* nombre,t_log* logger, t_conn* conn);
void consultar_con_memoria_espacio_disp();
t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad);

#endif /* SRC_CONSOLA_INICIAR_PROCESO_INICIAR_PROCESO_H_ */
