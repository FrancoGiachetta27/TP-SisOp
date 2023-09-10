#include "planificador.h"

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_list* lista_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb estado_EXEC;


/*uint32_t obtener_siguiente_pid(void) {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = siguientePID++;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}
*/
t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad){};


