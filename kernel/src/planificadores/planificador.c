#include "planificador.h"

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_list** cola_estado_BLOCKED;
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;

pthread_mutex_t estados_mutex;

sem_t grd_mult;
sem_t nuevoPcbEnSist;
sem_t nuevoPcbEnExit;
sem_t siguientePIDmutex;
uint32_t sig_PID;
uint32_t dispDeSalida;

uint32_t obtener_cantidad_dispositivos(char* dispositivos){
	printf("%s", dispositivos);
	char** array_elem = string_get_string_as_array(dispositivos);
	uint32_t cantidad = string_array_size(array_elem);
	return cantidad;
}

void atender_procesos_en_EXIT(){
	while(1){
		sem_wait(&nuevoPcbEnExit);
		t_pcb* pcbDeSalida = list_get(lista_estado_EXIT, 0);
		eliminar_proceso(pcbDeSalida);
	}
}

void planificador_largo_plazo() {
    pthread_t atenderExit;
    pthread_create(&atenderExit, NULL, (void*)atender_procesos_en_EXIT, NULL);
    pthread_detach(atenderExit);
    while(1) {
        sem_wait(&nuevoPcbEnSist);
        t_pcb* pcbAReady = list_get(lista_estado_NEW, 0);
		agregar_pcb_a_cola_READY(pcbAReady);
    }
}

void iniciar_planificadores(t_utils* config_kernel){
	pthread_t largoPlazoHilo;
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
	sem_init(&grd_mult,0,config_get_int_value(config_kernel->config, "GRADO_MULTIPROGRAMACION_INI"));
	pthread_create(&largoPlazoHilo, NULL, (void*)planificador_largo_plazo, NULL);
    pthread_detach(largoPlazoHilo);
}

uint32_t obt_sig_PID() {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = sig_PID + 1;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}

void agregar_pcb_a_cola_NEW(t_pcb* pcb){
	pthread_mutex_lock(&estados_mutex);
	list_add(lista_estado_NEW, pcb);
	pthread_mutex_unlock(&estados_mutex);
	sig_PID = obt_sig_PID();
}

void agregar_pcb_a_cola_READY(t_pcb* pcb){
	sem_wait(&grd_mult);
	pthread_mutex_lock(&estados_mutex);
	pcb = (t_pcb*)list_remove(lista_estado_NEW,0);
	list_add(lista_estado_READY,(void *) pcb);
	pcb->estado = "READY";
	pthread_mutex_unlock(&estados_mutex);
}
void agregar_pcb_a_cola_EXIT(t_pcb* pcb){
	//pensar manera de obtener la lista en la q se encuentra el pcb
}

// Pasarlos a tests!!!
void prueba_elementos_cola_bloqueados(){
	printf("\n Llego aca\n");
	t_pcb* nuevoPCB = crear_pcb(1,"name", 2,2);
	list_add(cola_estado_BLOCKED[0], nuevoPCB);
	nuevoPCB = crear_pcb(4,"name",2,2);
	list_add(cola_estado_BLOCKED[0], nuevoPCB);
	nuevoPCB = crear_pcb(2,"name",2,2);
	list_add(cola_estado_BLOCKED[1], nuevoPCB);
	nuevoPCB = crear_pcb(3,"name",2,2);
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
	t_pcb* nuevoPCB = crear_pcb(sig_PID,"name",2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	nuevoPCB = crear_pcb(sig_PID,"name",2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	nuevoPCB = crear_pcb(sig_PID,"name",2,2);
	list_add(lista_estado_NEW, nuevoPCB);
	sig_PID = obt_sig_PID();
	uint32_t cantElem = list_size(lista_estado_NEW);
	int i;
	for (i = 0; i <= cantElem - 1; i++){
		t_pcb* aMostrar = list_get(lista_estado_NEW,i);
		printf("\n ID de proceso = %d\n", aMostrar->pid);
	}
}