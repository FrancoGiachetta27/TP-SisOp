#include "planificador.h"

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_list** cola_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;

uint32_t sig_PID;
uint32_t dispDeSalida;

void avisar_a_memoria_nuevo_proceso(uint32_t id, uint32_t tamanio, char* nombre,t_log* logger, t_conn* conn){
	/*char* nombreArchivo = string_from_format("%s.%s",nombre, "txt" );
	printf("\n%s", nombreArchivo);
	t_package* nuevoPaquete = create_empty_package(CREATE_PROCESS);
	void* buffer = serializar_proceso_nuevo(id, tamanio, nombre);
	int tamanioBuffer = 2 * sizeof(uint32_t) + string_length(nombre) + 1;
	nuevoPaquete->buffer = buffer;
	send_package(nuevoPaquete, conn->memory_socket, logger);
	destroy_package(nuevoPaquete);*/
};

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

	dispDeSalida = obtener_cantidad_dispositivos(config_get_string_value(config_kernel->config,"INSTANCIAS_RECURSOS"));
	printf("cantidad de dispositivos = %d", dispDeSalida);
	for(int i = 0; i <= dispDeSalida - 1; i++){
		cola_estado_BLOCKED[i] = list_create();
	}
}

uint32_t obt_sig_PID() {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = sig_PID + 1;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}

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

t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad){
	consultar_con_memoria_espacio_disp();
	t_pcb* nuevoPCB = crear_pcb(sig_PID, atoi(tamanio), atoi(prioridad));
	agregar_pcb_a_cola_NEW(nuevoPCB);
	return nuevoPCB;
}

void listar_proceso(t_list* lista){
	int i = 0;
	t_pcb* pcb;
	int cantElem = lista->elements_count;
	for(i = 0; i <= cantElem - 1; i++){
		pcb = list_get(lista,i);
		printf("Proceso : %d\n", pcb->pid);
	}
}

void listar_procesos_por_estado(){
	int i;
	printf("Listado procesos en NEW:\n");
	listar_proceso(lista_estado_NEW);
	printf("Listado procesos en READY:\n");
	listar_proceso(lista_estado_READY);
	printf("Listado procesos en EXIT:\n");
	listar_proceso(lista_estado_EXIT);
	printf("Proceso en EXECUTE: %d", estado_EXEC->pid);
	uint32_t dispSalidaAux = dispDeSalida;
	for(int i = 0; i <= dispDeSalida - 1; i++){
		printf("Listado procesos en BLOCKED disp %d", i);
		listar_proceso(cola_estado_BLOCKED[i]);
	}

}

void finalizar_proceso(){};

void detener_planificacion(){};

void reiniciar_planificacion(){};

void cambiar_grado_multiprogramacion(){};

void consultar_con_memoria_espacio_disp(){};

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
	t_pcb* nuevoPCB = crear_pcb(sig_PID,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	nuevoPCB = crear_pcb(sig_PID,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	nuevoPCB = crear_pcb(sig_PID,2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	uint32_t cantElem = list_size(lista_estado_NEW);
	int i;
	for (i = 0; i <= cantElem - 1; i++){
		t_pcb* aMostrar = list_get(lista_estado_NEW,i);
		printf("\n ID de proceso = %d\n", aMostrar->pid);
	}
}
