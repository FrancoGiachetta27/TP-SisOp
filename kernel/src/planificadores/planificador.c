#include "planificador.h"

typedef struct t_pcb {

    uint32_t pid;
    uint32_t tamanio;
    uint32_t programCounter;
    uint32_t prioridad;
    char* nom_arch_inst;

}t_pcb;


t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_list** cola_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;

void consultar_con_memoria_espacio_disp(){}; //Usar un bool con un if
void avisar_a_memoria_nuevo_proceso(){};

uint32_t obtener_cantidad_dispositivos(char* dispositivos){
	printf("%s", dispositivos);
	char** array_elem = string_get_string_as_array(dispositivos);
	uint32_t cantidad = string_array_size(array_elem);
	return cantidad;
}
void iniciar_planificadores(t_utils* config_kernel){
	sig_PID = 1;
	lista_estado_NEW =  list_create();
	lista_estado_READY = list_create();
	lista_estado_EXIT = list_create();
	cola_estado_BLOCKED = list_create();

	uint32_t dispDeSalida = obtener_cantidad_dispositivos(config_get_string_value(config_kernel->config,"INSTANCIAS_RECURSOS"));
	printf("cantidad de dispositivos = %d", dispDeSalida);
	for(int i = 0; i <= dispDeSalida - 1; i++){
		cola_estado_BLOCKED[i] = list_create();
	}
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


void agregar_pcb_a_cola_NEW(t_pcb* pcb){
	pthread_mutex_t colaNEW_mutex;
	pthread_mutex_lock(&colaNEW_mutex);
	list_add(lista_estado_NEW, pcb);
	pthread_mutex_unlock(&colaNEW_mutex);
	sig_PID = obt_sig_PID();
}



void crear_proceso(char* source, char* tamanio, char* prioridad){
	consultar_con_memoria_espacio_disp();
	t_pcb* nuevoPCB = crear_pcb(sig_PID, atoi(tamanio), atoi(prioridad));
	agregar_pcb_a_cola_NEW(nuevoPCB);
	avisar_a_memoria_nuevo_proceso();
}
void listar_procesos_por_estado(){}

void prueba_elementos_cola_bloqueados(){
	printf("\n Llego aca\n");
	t_pcb* nuevoPCB = crear_pcb(1,2,2);
	list_add(cola_estado_BLOCKED[0], nuevoPCB);
	nuevoPCB = crear_pcb(4,2,2);
	list_add(cola_estado_BLOCKED[0], nuevoPCB);
	nuevoPCB = crear_pcb(2,2,2);
	list_add(cola_estado_BLOCKED[1], nuevoPCB);
	nuevoPCB = crear_pcb(3,2,2);
	list_add(cola_estado_BLOCKED[2], nuevoPCB);
	uint32_t cantElem0 = list_size(cola_estado_BLOCKED[0]);
	uint32_t cantElem1 = list_size(cola_estado_BLOCKED[1]);
	uint32_t cantElem2 = list_size(cola_estado_BLOCKED[2]);
	int i, j, k;
	printf("\n Cola BLOQUEADOS 0 = \n");
	for (i = 0; i <= cantElem0 - 1; i++){

		t_pcb* aMostrar = list_get(cola_estado_BLOCKED[0],i);
		printf("ID de proceso = %d\n", aMostrar->pid);
	}
	printf("\n Cola BLOQUEADOS 1 = \n");
	for (j = 0; j <= cantElem1 - 1; j++){
		t_pcb* aMostrar = list_get(cola_estado_BLOCKED[1],j);
		printf("ID de proceso = %d\n", aMostrar->pid);
	}
	printf("\n Cola BLOQUEADOS 2 = \n");
	for (k = 0; k <= cantElem2 - 1; k++){
		t_pcb* aMostrar = list_get(cola_estado_BLOCKED[2],k);
		printf("ID de proceso = %d\n", aMostrar->pid);
	}
}

void prueba_agregar_proceso_a_NEW(){
	printf("\n Llego aca\n");
	t_pcb* nuevoPCB = crear_pcb(1,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	nuevoPCB = crear_pcb(2,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	nuevoPCB = crear_pcb(3,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	uint32_t cantElem = list_size(lista_estado_NEW);
	int i;
	for (i = 0; i <= cantElem - 1; i++){
		t_pcb* aMostrar = list_get(lista_estado_NEW,i);
		printf("\n ID de proceso = %d\n", aMostrar->pid);
	}
}
