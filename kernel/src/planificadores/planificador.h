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

#define PLANNING_THREADS_INITIAL_VALUE -4

typedef struct {
    int instances;
    t_dictionary* pids;
    t_list* blocked_list;
} t_block;

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_dictionary* colas_BLOCKED; 
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;
sem_t grd_mult;
sem_t planificadores_terminados;
sem_t proceso_en_cola_ready;
sem_t finish_interrupted_process;
sem_t executing_process;
sem_t freed_resource;
sem_t process_in_exit;
sem_t process_in_new;
pthread_mutex_t cola_exit;
pthread_mutex_t cola_new;
pthread_mutex_t cola_ready;
pthread_mutex_t mtx_execute_process;
char* algoritmo;
bool working;
int actual_grd_mult;

void iniciar_estructuras_planificadores(t_utils* utils);
void terminar_estructuras_planificadores();
void destroy_executing_process();
void modify_executing_process(t_pcb* pcb);
void send_to_exit(t_pcb* pcb, t_log* logger, int end_state);
t_pcb* encontrar_proceso_por_PID(uint32_t pid);
uint32_t obt_sig_PID();
void execute_ready_process(t_pcb* pcb, t_log* logger);

void agregar_pcb_a_cola_READY(t_pcb* pcb, t_log* logger);
char* mostrar_pids_en_ready(t_log* logger);

void prueba_agregar_proceso_a_NEW();
void eliminar_proceso(t_pcb* pcb);

#endif /* SRC_PLANIFICADORES_PLANIFICADOR_H_ */

