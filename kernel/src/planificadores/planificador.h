#ifndef SRC_PLANIFICADORES_PLANIFICADOR_H_
#define SRC_PLANIFICADORES_PLANIFICADOR_H_

#include <commons/collections/list.h>
#include <pcb/pcb.h>
#include <package/package.h>
#include <config/config.h>
#include <planificadores/codigos_de_operacion.h>
#include <semaphore.h>
#include <pthread.h>

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_list** cola_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;
uint32_t dispDeSalida;
sem_t nuevoPcbEnSist;
sem_t nuevoPcbEnExit;

uint32_t obtener_cantidad_dispositivos(char* dispositivos);
void  planificador_largo_plazo(void);
void iniciar_planificadores(t_utils* utils);
uint32_t obt_sig_PID();

void agregar_pcb_a_cola_NEW(t_pcb* pcb);
void agregar_pcb_a_cola_READY(t_pcb* pcb);
void agregar_pcb_a_cola_EXIT(t_pcb* pcb);

void prueba_agregar_proceso_a_NEW();

#endif /* SRC_PLANIFICADORES_PLANIFICADOR_H_ */

