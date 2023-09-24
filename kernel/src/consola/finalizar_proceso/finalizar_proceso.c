#include "finalizar_proceso.h"


void finalizar_proceso(uint32_t pid) {
	t_pcb* pcb = encontrar_proceso_por_PID(pid);
	agregar_pcb_a_cola_EXIT(pcb);
	sem_post(&nuevoPcbEnExit);
};

void eliminar_proceso(t_pcb* pcb) {
	//borrar estructuras, avisar a memoria
};

t_pcb* encontrar_proceso_por_PID(uint32_t pid) {

}