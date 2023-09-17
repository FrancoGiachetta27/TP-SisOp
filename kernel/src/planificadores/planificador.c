#include "planificador.h"

pthread_mutex_t siguientePIDmutex;

uint32_t sig_PID;

typedef struct t_pcb {

    uint32_t pid;
    uint32_t tamanio;
    uint32_t programCounter;
    uint32_t prioridad;
    char* nom_arch_inst;

}t_pcb;


t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_queue** cola_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb estado_EXEC;

void consultar_con_memoria_espacio_disp(){}; //Usar un bool con un if
void avisar_a_memoria_nuevo_proceso(){};

void iniciar_planificadores(){
	uint32_t sig_PID = 1;
	lista_estado_NEW =  list_create();
}

/*t_list* obtener_lista_de_instrucciones(char* path_archivo){
	//path archivo es el nombre del proceso
	FILE * rutaArchivoInstrucciones = fopen(path_archivo, "r");
	t_list* lista_inst;
	return lista_inst;
} */

t_pcb* crear_pcb(uint32_t pid, uint32_t tamanio, uint32_t prioridad){
	t_pcb* nuevoPCB = malloc(sizeof(*nuevoPCB));
	nuevoPCB->pid = pid;
	nuevoPCB->tamanio = tamanio;
	nuevoPCB->programCounter = 0;
	nuevoPCB->prioridad = prioridad;
	nuevoPCB->nom_arch_inst = "instruccionesxD";
	printf("\n Nuevo PCB creado con id %d,tamanio %d y prioridad %d \n", nuevoPCB->pid,nuevoPCB->tamanio,nuevoPCB->prioridad);
	return nuevoPCB;
}
void agregar_pcb_a_cola_NEW(){}

uint32_t obtener_siguiente_pid(void) {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = sig_PID++;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}

void crear_proceso(char* source, char* tamanio, char* prioridad){
	consultar_con_memoria_espacio_disp();
	//obtener_lista_de_instrucciones(source);
	//crear_pcb();
	agregar_pcb_a_cola_NEW();
	avisar_a_memoria_nuevo_proceso();
}

void prueba_agregar_proceso_a_NEW(){
	printf("\n Llego aca\n");
	t_pcb* nuevoPCB = crear_pcb(1,2,2);
	list_add(lista_estado_NEW, (void *)nuevoPCB);
	nuevoPCB = crear_pcb(2,2,2);
	list_add(lista_estado_NEW, (void *)nuevoPCB);
	nuevoPCB = crear_pcb(3,2,2);
	list_add(lista_estado_NEW, (void *)nuevoPCB);
	uint32_t cantElem = list_size(lista_estado_NEW);
	int i;
	for (i = 0; i <= cantElem - 1; i++){
		t_pcb* aMostrar = list_get(lista_estado_NEW,i);
		printf("\n ID de proceso = %d\n", aMostrar->pid);
	}
}
