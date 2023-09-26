#include "iniciar_proceso.h"

uint32_t sig_PID;

void iniciar_proceso(char* source, char* size, char* priority, t_log* logger, t_conn* conn) {
    t_pcb* pcbCreado = crear_proceso(source, size, priority);
	avisar_a_memoria_nuevo_proceso(pcbCreado, logger, conn);
}

void avisar_a_memoria_nuevo_proceso(t_pcb* pcb, t_log* logger, t_conn* conn) {
	t_package* nuevoPaquete = create_empty_package(CREATE_PROCESS);
	void* buffer = serialize_pcb(pcb);
	nuevoPaquete->buffer = buffer;
	nuevoPaquete->size = serialized_pcb_size(pcb->nom_arch_inst);
	send_package(nuevoPaquete, conn->memory_socket, logger);
};

void consultar_con_memoria_espacio_disp() {}

t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad){
	consultar_con_memoria_espacio_disp();
	t_pcb* nuevoPCB = crear_pcb(sig_PID, source, atoi(tamanio), atoi(prioridad));
	agregar_pcb_a_cola_NEW(nuevoPCB);
	return nuevoPCB;
}

void agregar_pcb_a_cola_NEW(t_pcb* pcb) {
	pthread_mutex_lock(&estados_mutex);
	list_add(lista_estado_NEW, pcb);
	pthread_mutex_unlock(&estados_mutex);
	sig_PID = obt_sig_PID();
}

			