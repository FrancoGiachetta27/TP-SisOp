#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"
#include <commons/string.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include <config/config.h>

void iniciar_planificadores(t_utils* utils);
void avisar_a_memoria_nuevo_proceso();
void crear_proceso(char* source, char* tamanio, char* prioridad);
void prueba_agregar_proceso_a_NEW();
void listar_procesos_por_estado();
