#ifndef SRC_PLANIFICADORES_PLANIFICADOR_H_
#define SRC_PLANIFICADORES_PLANIFICADOR_H_

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <pcb/pcb.h>
#include <package/package.h>
#include <config/config.h>
#include <planificadores/codigos_de_operacion.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int instances;
    t_list* blocked_list;
} t_block;

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_dictionary* colas_BLOCKED; 
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;
sem_t grd_mult;
sem_t planificadores_terminados;
pthread_mutex_t estados_mutex;
bool working;

void iniciar_estructuras_planificadores(t_utils* utils);
void terminar_estructuras_planificadores();
t_pcb* encontrar_proceso_por_PID(uint32_t pid);
uint32_t obt_sig_PID();

void agregar_pcb_a_cola_READY(t_pcb* pcb, t_log* logger);

void prueba_agregar_proceso_a_NEW();

#endif /* SRC_PLANIFICADORES_PLANIFICADOR_H_ */

