#include "planificador.h"

t_list* lista_estado_NEW;
t_list* lista_estado_READY;
t_dictionary* colas_BLOCKED; 
t_list* lista_estado_EXIT;
t_pcb* estado_EXEC;

pthread_mutex_t estados_mutex;

sem_t grd_mult;
pthread_mutex_t siguientePIDmutex;
uint32_t sig_PID;
uint32_t dispDeSalida;
bool working = true;

void iniciar_planificadores(t_utils* config_kernel){
	sig_PID = 1;
	lista_estado_NEW = list_create();
	lista_estado_READY = list_create();
	lista_estado_EXIT = list_create();
	colas_BLOCKED = dictionary_create();
	char** resources = config_get_array_value(config_kernel->config,"RECURSOS");
	char** resources_instances = config_get_array_value(config_kernel->config,"INSTANCIAS_RECURSOS");
	for (int i = 0; resources[i] != NULL; i++)
	{
		t_block* block = malloc(sizeof(*block));
		block->instances = atoi(resources_instances[i]);
    	block->blocked_list = list_create();
		dictionary_put(colas_BLOCKED, resources[i], block);
	}
	sem_init(&grd_mult,0,config_get_int_value(config_kernel->config, "GRADO_MULTIPROGRAMACION_INI"));
}

t_pcb* encontrar_proceso_por_PID(uint32_t pid) {
    int _pcb_by_pid_in_list(t_pcb *pcb) {
        return pcb->pid == pid;
    };
	if (estado_EXEC->pid == pid) return estado_EXEC;

	t_pcb* pcb_en_new = list_find(lista_estado_NEW, (void*) _pcb_by_pid_in_list);
	if (pcb_en_new != NULL) return pcb_en_new;
	t_pcb* pcb_en_ready = list_find(lista_estado_READY, (void*) _pcb_by_pid_in_list);
	if (pcb_en_ready != NULL) return pcb_en_ready;
	t_pcb* pcb_en_exit = list_find(lista_estado_EXIT, (void*) _pcb_by_pid_in_list);
	if (pcb_en_exit != NULL) return pcb_en_exit;
	t_pcb* pcb_en_blocked = NULL;
	void _pcb_by_pid_in_dict(char* _, t_block* block) {
		t_pcb* pcb = list_find(block->blocked_list, (void*) _pcb_by_pid_in_list);
		if (pcb != NULL) {
			pcb_en_blocked = pcb;
		}
    };
	dictionary_iterator(colas_BLOCKED, (void*) _pcb_by_pid_in_dict);
	return pcb_en_blocked;
}

uint32_t obt_sig_PID() {
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = sig_PID + 1;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}

void agregar_pcb_a_cola_READY(t_pcb* pcb, t_log* logger){
	int previous_state = pcb->estado;
	sem_wait(&grd_mult);
	pthread_mutex_lock(&estados_mutex);
	list_add(lista_estado_READY, pcb);
	pcb->estado = READY;
	pthread_mutex_unlock(&estados_mutex);
	log_info(logger, "PID: %d - Estado Anterior: %d - Estado Actual: %d", pcb->pid, previous_state, READY);
}

// Pasarlos a tests!!!
// void prueba_elementos_cola_bloqueados(){
// 	printf("\n Llego aca\n");
// 	t_pcb* nuevoPCB = crear_pcb(1,"name", 2,2);
// 	list_add(colas_BLOCKED[0], nuevoPCB);
// 	nuevoPCB = crear_pcb(4,"name",2,2);
// 	list_add(colas_BLOCKED[0], nuevoPCB);
// 	nuevoPCB = crear_pcb(2,"name",2,2);
// 	list_add(colas_BLOCKED[1], nuevoPCB);
// 	nuevoPCB = crear_pcb(3,"name",2,2);
// 	list_add(colas_BLOCKED[2], nuevoPCB);
// 	uint32_t cantElem0 = list_size(colas_BLOCKED[0]);
// 	uint32_t cantElem1 = list_size(colas_BLOCKED[1]);
// 	uint32_t cantElem2 = list_size(colas_BLOCKED[2]);
// 	int i, j, k;
// 	printf("\n Cola BLOQUEADOS 0 = \n");
// 	for (i = 0; i <= cantElem0 - 1; i++){

// 		t_pcb* aMostrar = list_get(colas_BLOCKED[0],i);
// 		printf("ID de proceso = %d\n", aMostrar->pid);
// 	}
// 	printf("\n Cola BLOQUEADOS 1 = \n");
// 	for (j = 0; j <= cantElem1 - 1; j++){
// 		t_pcb* aMostrar = list_get(colas_BLOCKED[1],j);
// 		printf("ID de proceso = %d\n", aMostrar->pid);
// 	}
// 	printf("\n Cola BLOQUEADOS 2 = \n");
// 	for (k = 0; k <= cantElem2 - 1; k++){
// 		t_pcb* aMostrar = list_get(colas_BLOCKED[2],k);
// 		printf("ID de proceso = %d\n", aMostrar->pid);
// 	}
// }

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
