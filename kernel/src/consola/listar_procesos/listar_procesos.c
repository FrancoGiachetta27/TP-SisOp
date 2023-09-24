#include "listar_procesos.h"

void listar_proceso(t_list* lista){
	int i = 0;
	t_pcb* pcb;
	int cantElem = lista->elements_count;
	for(i = 0; i <= cantElem - 1; i++){
		pcb = list_get(lista,i);
		printf("Proceso : %d\n", pcb->pid);
	}
}

void listar_procesos_por_estados(){
	printf("Listado procesos en NEW:\n");
	listar_proceso(lista_estado_NEW);
	printf("Listado procesos en READY:\n");
	listar_proceso(lista_estado_READY);
	printf("Listado procesos en EXIT:\n");
	listar_proceso(lista_estado_EXIT);
	printf("Proceso en EXECUTE: %d", estado_EXEC->pid);
	for(int j = 0; j <= dispDeSalida - 1; j++){
		printf("Listado procesos en BLOCKED disp %d", j);
		listar_proceso(cola_estado_BLOCKED[j]);
	}

}