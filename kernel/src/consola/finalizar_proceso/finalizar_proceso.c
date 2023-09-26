#include "finalizar_proceso.h"

void finalizar_proceso(uint32_t pid) {
	t_pcb* pcb = encontrar_proceso_por_PID(pid);
	eliminar_proceso(pcb);
};